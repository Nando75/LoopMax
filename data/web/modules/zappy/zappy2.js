//IMPORT LOOPMAX SERVICES:
import { translator, loader, toast, fetchAPI, sendAPI, modules } from '../../core/js/LoopMaxUtils.js';

export default class Zappy2 {
    constructor(config = {}) {
        this.modules = modules();
        this.data = this.modules.get("zappy2");
        this.state = {
            invert: 0,
            relays: {}   // key: pin.number, value: 0/1
        };

        this.translations = {
            "msgInvert":      { it: "Invertire lo stato dei relè?", en: "Invert relè status?" },
            "msgCmdExecuted": { it: "Comando eseguito", en: "Command executed" },
            "lblDeviceName":  { it: "Nome dispositivo", en: "Device name" },
            "lblInvert":      { it: "Inverti stato", en: "Invert status" },
            "lblSave":        { it: "Salva", en: "Save" },
            "lblPins":        { it: "Uscite", en: "Outputs" },
            "msgDevSaved":    { it: "Nome dispositivo salvato", en: "Device name saved" },
            "msgPinSaved":    { it: "Nome salvato", en: "Name saved" }
        };
        translator.addTranslations(this.translations);
    }

    getHtml() {
        const deviceName = this.data.DeviceName || this.data.Name || "Zappy2";
        const pins = Array.isArray(this.data.pins) ? this.data.pins : [];
        const hasMultiplePins = pins.length > 1;
            const pinsHtml = pins.map((pin, idx) => {
                const ch = pin.number;
                const label = pin.name || `Relè ${idx + 1}`;
                const isOn = pin.level ? "on" : "";

                return `
                    <div class="relay-switch" data-pin-number="${ch}">
                        <div class="flex-grow-1 me-3">
                            <input type="text"
                                class="form-control form-control-sm pin-name-input"
                                value="${label}"
                                data-pin-number="${ch}">
                        </div>

                        <div class="big-switch relay-toggle ${isOn}"
                            data-channel="${ch}"
                            data-pin-number="${ch}"
                            id="relay-${ch}">
                        </div>

                        <button class="btn btn-sm btn-outline-primary ms-2 pin-save-btn"
                                data-pin-number="${ch}">
                            ${translator.tr("lblSave")}
                        </button>
                    </div>
                `;
            }).join("");

        const invertBoxHtml = hasMultiplePins ? `
            <div class="invert-box">
                <label class="form-label fw-bold" data-tr="lblInvert"></label>
                <div id="invertSwitch" class="big-switch"></div>
            </div>
        ` : "";

        return `${this.getCss()}
            <div class="container py-3">
                <div class="relay-card">
                    <div class="mb-3 d-flex align-items-end gap-2">
                        <div class="flex-grow-1">
                            <label class="form-label fw-bold" data-tr="lblDeviceName"></label>
                            ${this.data.Icon}
                            <input type="text" 
                                   id="deviceNameInput" 
                                   class="form-control" 
                                   value="${deviceName}">
                        </div>
                        <button id="deviceNameSaveBtn" 
                                class="btn btn-sm btn-outline-primary ms-2" data-tr="lblSave">
                            
                        </button>
                    </div>
                    
                    <div class="mb-2 fw-bold" data-tr="lblPins"></div>
                    ${pinsHtml}

                    ${invertBoxHtml}
                </div>
            </div>`;
    }

initObjects() {
    const pins = Array.isArray(this.data.pins) ? this.data.pins : [];

    /* ===============================
       Device name
    =============================== */
    const devInput = document.getElementById("deviceNameInput");
    const devSaveBtn = document.getElementById("deviceNameSaveBtn");

    if (devInput && devSaveBtn) {
        devSaveBtn.addEventListener("click", async () => {
            const newName = devInput.value.trim();
            if (!newName) return;
            await this.saveDeviceName(newName);
        });
    }

    /* ===============================
       Invert (azione)
    =============================== */
    const invertSwitch = document.getElementById("invertSwitch");
    if (invertSwitch) {
        invertSwitch.addEventListener("click", () => {
            this.invert();
        });
    }

    /* ===============================
       Relay toggle
    =============================== */
    document.querySelectorAll(".relay-toggle").forEach(el => {
        const ch = Number(el.dataset.channel);

        // stato iniziale già renderizzato in HTML → NON ricalcolare
        this.state.relays[ch] = el.classList.contains("on") ? 1 : 0;

        el.addEventListener("click", () => {
            const state = el.classList.contains("on") ? 0 : 1;

            // UI ottimistica
            el.classList.toggle("on", state === 1);
            this.state.relays[ch] = state;

            this.change(ch, state);
        });
    });

    /* ===============================
       Save pin name
    =============================== */
    document.querySelectorAll(".pin-save-btn").forEach(btn => {
        btn.addEventListener("click", async () => {
            const pinNumber = Number(btn.dataset.pinNumber);

            const input = document.querySelector(
                `.pin-name-input[data-pin-number="${pinNumber}"]`
            );
            if (!input) return;

            const newName = input.value.trim();
            if (!newName) return;

            await this.savePinName(pinNumber, newName);
        });
    });
}


    getCss() {
        return `<style>
            .relay-card {
                border-radius: 14px;
                padding: 20px;
                background: var(--bs-card-bg);
                color: var(--bs-body-color);
                box-shadow: 0 4px 12px rgba(0,0,0,0.08);
            }

            .relay-title {
                font-size: 1.6rem;
                font-weight: 700;
                margin-bottom: 15px;
                color: var(--bs-body-color);
            }

            .relay-switch {
                display: flex;
                align-items: center;
                justify-content: space-between;
                padding: 10px 12px;
                border-radius: 12px;
                background: var(--bs-card-bg);
                border: 1px solid var(--bs-border-color);
                margin-bottom: 10px;
                transition: 0.2s ease;
            }

            .relay-switch:hover {
                transform: translateY(-2px);
                box-shadow: 0 4px 10px rgba(0,0,0,0.15);
            }

            .big-switch {
                width: 60px;
                height: 32px;
                background: var(--bs-secondary-bg);
                border-radius: 20px;
                position: relative;
                cursor: pointer;
                transition: background 0.25s;
            }

            .big-switch::after {
                content: "";
                width: 28px;
                height: 28px;
                background: var(--bs-body-bg);
                border-radius: 50%;
                position: absolute;
                top: 2px;
                left: 2px;
                transition: 0.25s;
                box-shadow: 0 2px 4px rgba(0,0,0,0.2);
            }

            .big-switch.on {
                background: var(--bs-success);
            }

            .big-switch.on::after {
                left: 30px;
            }

            .invert-box {
                margin-bottom: 20px;
                padding: 12px 18px;
                background: var(--bs-card-bg);
                border: 1px solid var(--bs-border-color);
                border-radius: 12px;
            }
        </style>`;
    }


async invert() {
    const invertSwitch = document.getElementById("invertSwitch");
    if (!invertSwitch) return;
    if (!confirm(translator.tr("msgInvert"))) return;

    loader(true);

    try {
        // 1️⃣ chiamata ESP (source of truth)
        const json = await fetchAPI(this.data.Name + "/invert");

        if (!json || !Array.isArray(json.pins)) {
            throw new Error("Risposta ESP non valida");
        }

        // 2️⃣ aggiorna UI in base allo stato REALE
        json.pins.forEach(pin => {
            const toggle = document.getElementById(`relay-${pin.number}`);
            if (!toggle) return;

            toggle.classList.toggle("on", pin.level === 1);
        });

        // 3️⃣ aggiorna stato interno
        this.state.pins = json.pins;
        
        // 4️⃣ aggiorna invertSwitch (opzionale)
        const anyOn = json.pins.some(p => p.level === 1);
        invertSwitch.classList.toggle("on", anyOn);

        //Aggiorna il tabs modules:
        this.modules.refreshPinStatus(this.data.Name, json.pins);


    } catch (err) {
        console.error(err);
        toast(err.message || err, "danger");
    } finally {
        loader(false);
    }
}



  async change(channel, value) {
        loader(true);
        const toggle = document.querySelector(`[data-channel="${channel}"]`);
        if (!toggle) return;
        const previous = toggle.classList.contains("on");
        const form = new URLSearchParams();
        form.append("channel", channel);
        form.append("value", value);
        try {
            await sendAPI(this.data.Name + "/set", {
                method: "POST",
                body: form.toString(),
                return: "text"
            });
            const pins = Array.isArray(this.data.pins) ? this.data.pins : [];
            const pin = pins.find(p => p.number === channel);
            if (!pin) return;
            pin.level = value;
            /*
            const input = document.querySelector(`.pin-name-input[data-pin-index="${idx}"]`);
            if (input) {
                const value = input.value;
                console.log(value);
            }
            */
            this.modules.updatePin(this.data.Name, channel, value, pin.name);
            this.modules.refreshPinStatus(this.data.Name, pins);

        } catch (err) {
            console.error(err);
            toggle.classList.toggle("on", previous);
            toast(err, "danger");
        } finally { loader(false); }
    }


    // ====== CALLBACK CONFIG (DA AGGANCIARE ALLE TUE API) ======

    async saveDeviceName(newName) {
        loader(true);
        const form = new URLSearchParams();
        form.append("name", newName);
        try {
            await sendAPI(this.data.Name + "/edit/devName", {
                method: "POST",
                body: form.toString(),
                return: "text"
            });
            
            this.modules.DeviceName = newName;
            this.data.DeviceName = newName;
            toast(translator.tr("msgDevSaved"));
        } catch (err) {
            console.error(err);
            toggle.classList.toggle("on", previous);
            toast(err, "danger");
        } finally { loader(false); }





        
        
    }

    async savePinName(pinNumber, newName) {
            loader(true);
            const form = new URLSearchParams();
            form.append("number", pinNumber);
            form.append("name", newName);
            try {
                await sendAPI(this.data.Name + "/edit/pinName", {
                    method: "POST",
                    body: form.toString(),
                    return: "text"
                });
                const pin = this.data.pins.find(p => p.number === pinNumber);
                if (!pin) return;
                pin.name = newName;
                this.modules.updatePin(this.data.Name, pinNumber, pin.level, newName);
                this.modules.refreshPinStatus(this.data.Name, this.data.pins);
                toast(translator.tr("msgPinSaved"));
            } catch (err) {
                console.error(err);
                toggle.classList.toggle("on", previous);
                toast(err, "danger");
            } finally { loader(false); }
            
        }





}
