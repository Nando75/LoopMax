//IMPORT LOOPMAX SERVICES:
import { translator, loader, toast, fetchAPI, sendAPI, modules, wifi, WebAppUrl, system } from '../../core/js/LoopMaxUtils.js';
//import  PinScheduler  from '../PinScheduler.js';
import PinScheduler from '../PinScheduler.js';



export default class Zappy2 {
    constructor(config = {}) {
        this.modules = modules();
        this.wifi = wifi();
        this.system = system();

        this.WebAppUrl = WebAppUrl();
        this.data = this.modules.get("zappy2");
        this.PinScheduler = new PinScheduler(this.data);

        this.state = {
            invert: 0,
            relays: {}   // key: pin.number, value: 0/1
        };

      this.translations = {
                "msgInvert":      { it:"Invertire lo stato dei relè?", en:"Invert relay status?", es:"¿Invertir el estado del relé?" },
                "msgCmdExecuted": { it:"Comando eseguito", en:"Command executed", es:"Comando ejecutado" },
                "lblDeviceName":  { it:"Nome dispositivo", en:"Device name", es:"Nombre del dispositivo" },
                "lblInvert":      { it:"Inverti stato", en:"Invert status", es:"Invertir estado" },
                "lblClose":       { it:"Chiudi", en:"Close", es:"Cerrar" },
                "lblPins":        { it:"Uscite", en:"Outputs", es:"Salidas" },
                "msgDevSaved":    { it:"Nome dispositivo salvato", en:"Device name saved", es:"Nombre del dispositivo guardado" },
                "msgPinSaved":    { it:"Nome salvato", en:"Name saved", es:"Nombre guardado" },
                "lblRegister":    { it:"Registra", en:"Register", es:"Registrar" },
                "lblRegisterDevice": { it:"Registra il dispositivo", en:"Register your device", es:"Registrar el dispositivo" }
            };

        translator.addTranslations(this.translations);
    }

    getHtml() {
        const deviceName = this.data.DeviceName || this.data.Name || "Zappy2";
        const update = this.data.UpdateAvalaible || false;
        //const isRegistered = this.data.isRegistered || false;
        const internet = this.wifi.internet || false;
        //const lblUpdate = translator.tr("lblUpdateFw")
        //const lblCancel = translator.tr("lblCancel");
        const pins = Array.isArray(this.data.pins) ? this.data.pins : [];
        const hasMultiplePins = pins.length > 1;
            const pinsHtml = pins.map((pin, idx) => {
                const ch = pin.number;
                const label = pin.name || `Relè ${idx + 1}`;
                const isOn = pin.level ? "on" : "";
                const btSchedule = internet ?  this.PinScheduler.getScheduleButton(ch) : ``;
                return `
                    <div class="relay-switch row align-items-center g-2 gx-0 w-100 mx-0" data-pin-number="${ch}">
                        <div class="col-12 col-md-3 d-flex justify-content-center">
                            <div class="big-switch relay-toggle ${isOn}"
                                data-channel="${ch}"
                                data-pin-number="${ch}"
                                id="relay-${ch}">
                            </div>
                        </div>
                        <div class="col-12 col-md-4">
                            <input type="text"
                                class="form-control form-control-sm pin-name-input"
                                value="${label}"
                                data-pin-number="${ch}">
                        </div>
                        <div class="col-12 col-md-5 d-flex flex-column flex-sm-row justify-content-end gap-2">
                            <button class="btn btn-sm btn-outline-primary pin-save-btn secondFont flex-fill"
                                    data-pin-number="${ch}" disabled data-tr="lblSave">
                                ${translator.tr("lblSave")}
                            </button>
                            ${btSchedule}
                        </div>
                    </div>
                `;
            }).join("");


        const invertBoxHtml = hasMultiplePins ? `
            <div class="invert-box">
                <label class="form-label fw-bold secondFont" data-tr="lblInvert"></label>
                <div id="invertSwitch" class="big-switch"></div>
            </div>
        ` : "";


        const updateBoxHtml = ``;
        /*
        const updateBoxHtml = update ? `
            <div class="mb-2 invert-box">
                    <div class="d-flex gap-2">
                        <label class="form-label fw-bold textGlow" data-tr="lblUpdateFw"></label>
                        <label class="form-label fw-bold textGlow">${this.data.newFwVersion}</label>
                    </div>

                    <button id="btUpdateFw"
                            type="button"
                            class="btn btn-warning mt-2 w-100"
                            data-tr="lblUpdateFw"
                            data-bs-toggle="modal"
                            data-bs-target="#modalUpdate">
                        ${lblUpdate}
                    </button>
                </div>

        ` : "";
        */
        
            
        const registerBoxHtml = ``;
            /*
            const registerBoxHtml = !isRegistered ? `
                <div class="mb-2 invert-box">
                    <div class="d-flex gap-2">
                        <label class="form-label fw-bold textGlow" data-tr="lblRegisterDevice"></label>
                    </div>

                    <button id="btRegisterDevice"
                            type="button"
                            class="btn btn-secondary mt-2 w-100">
                        ${translator.tr("lblRegister")}
                    </button>

                    <!-- Device key box -->
                    <div class="mt-3 small text-muted">
                        <div class="mb-1" data-tr="lblKey"></div>

                         <div class="d-flex gap-2">
                            <label id="deviceKeyInput" class="form-label fw-bold textGlowOrange KeyText" style="">
                                ${this.system.key}
                            </label>
                            <button class="btn btn-outline-secondary"
                                    type="button"
                                    id="btCopyDeviceKey"
                                    title="Copy">🖋</button>
                        </div>
                    </div>


                </div>
            ` : "";
            */


        const updateModalHtml = ``;
        /*
        const updateModalHtml = update ? `
                    <div class="modal fade" id="modalUpdate" data-bs-backdrop="static" data-bs-keyboard="false" tabindex="-1" aria-labelledby="modalUpdateLabel" aria-hidden="true">
                        <div class="modal-dialog modal-dialog-centered">
                            <div class="modal-content">
                            <div class="modal-header">
                                <h1 class="modal-title fs-5" id="modalUpdateLabel" data-tr="lblUpdateFw">${lblUpdate}</h1>
                                <button type="button" id="btUpdateClose1" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                            </div>
                            <div class="modal-body" id='modalUpdateBody' data-tr="msgUpdateFw"></div>
                            <div class="modal-footer">
                                <button type="button" id="btUpdateClose2" class="btn btn-secondary" data-bs-dismiss="modal" data-tr="lblCancel">${lblCancel}</button>
                                <button type="button" id="btUpdate" class="btn btn-primary" data-tr="lblUpdateFw">${lblUpdate}</button>
                            </div>
                            </div>
                        </div>
                    </div>
        ` : "";
        */




        const scheduleModalHtml = internet ? this.PinScheduler.getHtml() : "";
        

        return `${this.getCss()}
            <div class="container py-3">
                <div class="relay-card">
                    <div class="mb-3 d-flex align-items-end gap-2">
                        <div class="flex-grow-1">
                            <label class="form-label fw-bold secondFont" data-tr="lblDeviceName"></label>
                            ${this.data.Icon}
                            <input type="text" 
                                   id="deviceNameInput" 
                                   class="form-control" 
                                   value="${deviceName}">
                        </div>
                        <button id="deviceNameSaveBtn" 
                                class="btn btn-sm btn-outline-primary ms-2" data-tr="lblSave" disabled>
                            
                        </button>
                    </div>
                    
                    <div class="mb-2 fw-bold secondFont" data-tr="lblPins"></div>
                    ${pinsHtml}

                    ${invertBoxHtml}
                    
                    ${updateBoxHtml}

                    ${registerBoxHtml}

                </div>
            </div>
            ${updateModalHtml}
            
            ${scheduleModalHtml}`;

    }

initObjects() {
    const pins = Array.isArray(this.data.pins) ? this.data.pins : [];
    const internet = this.wifi.internet || false;
    const update = this.data.UpdateAvalaible || false;
    const isRegistered = this.data.isRegistered || false;
    /* ===============================
       Device name
    =============================== */
    const devInput = document.getElementById("deviceNameInput");
    const devSaveBtn = document.getElementById("deviceNameSaveBtn");

    if (devInput && devSaveBtn) {
         devInput.addEventListener("input", () => {
                devSaveBtn.disabled = false;
                devSaveBtn.classList.add("pending");   // evidenzia il bottone
            });


        devSaveBtn.addEventListener("click", async () => {
            const newName = devInput.value.trim();
            if (!newName) return;
            await this.saveDeviceName(newName);
            devSaveBtn.disabled = true;
            devSaveBtn.classList.remove("pending");
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
        // BLOCCO UI SE PIN È SOTTO PROGRAMMA 
           if (internet && this.isPinLocked(ch)) { 
             el.classList.add("disabled"); 
             el.style.pointerEvents = "none"; 
             el.style.opacity = "0.5"; 
           }
           else
           {
            this.state.relays[ch] = el.classList.contains("on") ? 1 : 0;
           }


        el.addEventListener("click", () => {
            if (internet && this.isPinLocked(ch)) 
                { toast("Line is scheduled"); return; }
            
            const state = el.classList.contains("on") ? 0 : 1;

            // UI ottimistica
            el.classList.toggle("on", state === 1);
            this.state.relays[ch] = state;

            this.change(ch, state);
        });
    });

   
   document.querySelectorAll(".pin-save-btn").forEach(btn => {
        const pinNumber = Number(btn.dataset.pinNumber);
        const input = document.querySelector(
            `.pin-name-input[data-pin-number="${pinNumber}"]`
        );
        // Quando cambia il testo → abilita il bottone
        if (input) {
            input.addEventListener("input", () => {
                btn.disabled = false;
                btn.classList.add("pending");   // evidenzia il bottone
            });
        }
        // Quando clicco Salva
        btn.addEventListener("click", async () => {
            const input = document.querySelector(
                `.pin-name-input[data-pin-number="${pinNumber}"]`
            );
            if (!input) return;
            const newName = input.value.trim();
            if (!newName) return;
            await this.savePinName(pinNumber, newName);
            // Dopo il salvataggio → torna normale
            btn.disabled = true;
            btn.classList.remove("pending");
        });

    });


    /*
    if(update)
    {
        const modalUpdate = document.getElementById('modalUpdate');
        if(modalUpdate)
        {
            modalUpdate.addEventListener('show.bs.modal', () => {
            const modalBody = document.getElementById("modalUpdateBody");
                if(modalBody)
                {
                    modalBody.innerHTML = translator.tr("msgUpdateFw");
                }
            });
            }
                const btUpdate = document.getElementById("btUpdate");
            if (btUpdate) {
                btUpdate.onclick = async () => {
                    btUpdate.disabled = true;
                    await this.updateFirmware();
                };
            }
    }
    */

    /*
    if(!isRegistered)
    {
        const btRegisterDevice = document.getElementById("btRegisterDevice");
        if(btRegisterDevice) btRegisterDevice.addEventListener("click", () => { this.openCloud(); });
        const copyBtn = document.getElementById("btCopyDeviceKey");
        if (copyBtn) copyBtn.addEventListener("click", () => { this.copyKey(copyBtn); });
    }
    */


    if(internet) this.PinScheduler.initObjects();
    
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
           .pending {
              border-color: var(--bs-warning);
                color: var(--bs-warning);
                font-weight: bold;
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
            if (this.isPinLocked(pin.number)) return;
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
            await sendAPI(this.data.Name +"/edit/devName", {
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

        async updateFirmware() {
            try {
                const json = await fetchAPI(this.data.Name + "/fwUpdate");
            } catch (err) {
                console.error(err);
                toast(err.message || err, "danger");
            } 
            

        }


        isPinLocked(pinNumber) {
                let cfg = {};
                try { cfg = JSON.parse(this.data.JsonConfig || "{}"); }
                catch { return false; }

                const schedules = cfg.schedules || {};
                const list = schedules[pinNumber];
                if (!list || list.length === 0) return false;

                const now = new Date();
                const nowMinutes = now.getHours() * 60 + now.getMinutes();
                const nowUnix = Math.floor(now.getTime() / 1000);

                for (const s of list) {
                    if (s.type === "weekly") {
                        const day = now.getDay(); // 0=Sun,1=Mon...
                        const bit = 1 << day;

                        if (s.days & bit) {
                            if (s.on <= s.off) {
                                if (nowMinutes >= s.on && nowMinutes < s.off) return true;
                            } else {
                                // fascia overnight
                                if (nowMinutes >= s.on || nowMinutes < s.off) return true;
                            }
                        }
                    }
                    /*
                    if (s.type === "weekly") {
                        const day = now.getDay(); // 0=Sun,1=Mon...
                        const bit = 1 << day;

                        if (s.days & bit) {
                            if (nowMinutes >= s.on && nowMinutes < s.off) {
                                return true;
                            }
                        }
                    }
                    */
                    if (s.type === "onetime") {
                        if (nowUnix >= s.start && nowUnix < s.end) {
                            return true;
                        }
                    }
                }

                return false;
            }

              openCloud() {
                const isRegistered = this.data.isRegistered === true;
                if (isRegistered) return;
                const url = `${this.WebAppUrl}?key=${encodeURIComponent(this.system.key)}`;
                const win = window.open("about:blank", "_blank");
                win.location.href = url;
                win.focus();
            }

            copyKey(copyBtn)
            {
                const keySpan = document.getElementById("deviceKeyInput");
                if (!keySpan) return;
                const key = keySpan.textContent.trim();
                if (navigator.clipboard && navigator.clipboard.writeText) {
                    navigator.clipboard.writeText(key);
                } else {
                    const tmp = document.createElement("textarea");
                    tmp.value = key;
                    document.body.appendChild(tmp);
                    tmp.select();
                    document.execCommand("copy");
                    document.body.removeChild(tmp);
                }
                if (!copyBtn) return;
                copyBtn.innerText = "✓";
                setTimeout(() => { copyBtn.innerText = "🖋"; }, 3000);
            }

}
