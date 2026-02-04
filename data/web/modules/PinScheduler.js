//IMPORT LOOPMAX SERVICES:
import { translator, loader, toast, fetchAPI, sendAPI, registerTimer, time, setTextContent } from '../core/js/LoopMaxUtils.js';

export default class PinScheduler {
    constructor(moduleData) {
        this.data = moduleData;
        translator.register(this);
        this.time = time();

        this.state = {
            invert: 0,
            relays: {}   // key: pin.number, value: 0/1
        };

        this.translations = {
            "lblSchedule":       { it: "Programmazione",        en: "Schedule",             es: "Programación" },
            "lblEdit":           { it: "Modifica",              en: "Edit",                 es: "Editar" },
            "lblScheduleType":   { it: "Tipo Programmazione",   en: "Schedule type",        es: "Tipo de programación" },
            "lblWeekly":         { it: "Settimanale",           en: "Weekly",               es: "Semanal" },
            "lblOneTime":        { it: "Unica",                 en: "One time",             es: "Única" },
            "lblDayMon":         { it: "Lun.",                  en: "Mon.",                 es: "Lun." },
            "lblDayTue":         { it: "Mar.",                  en: "Tue.",                 es: "Mar." },
            "lblDayWed":         { it: "Mer.",                  en: "Wed.",                 es: "Mié." },
            "lblDayThu":         { it: "Gio.",                  en: "Thu.",                 es: "Jue." },
            "lblDayFri":         { it: "Ven.",                  en: "Fri.",                 es: "Vie." },
            "lblDaySat":         { it: "Sab.",                  en: "Sat.",                 es: "Sáb." },
            "lblDaySun":         { it: "Dom.",                  en: "Sun.",                 es: "Dom." },
            "lblTimeOn":         { it: "Ora Accensione",        en: "Power On Time",        es: "Hora de encendido" },
            "lblTimeOff":        { it: "Ora spegnimento",       en: "Power Off Time",       es: "Hora de apagado" },
            "lblDateTimeOn":     { it: "Data/Ora Accensione",   en: "Power On Date/Time",   es: "Fecha/Hora de encendido" },
            "lblDateTimeOff":    { it: "Data/Ora Spegnimento.", en: "Power Off Date/Time",  es: "Fecha/Hora de apagado" },
            "lblNextDay":        { it: "Giorno Successivo",     en: "Next Day",             es: "Día siguiente" },
            "lblDays":           { it: "Giorni",                en: "Days",                 es: "Días" },
            "lblTimeSlot":       { it: "Intervallo orario",     en: "Time slot",            es: "Franja horaria" },
            "lblStart":          { it: "Inizio",                en: "Start",                es: "Inicio" },
            "lblEnd":            { it: "Fine",                  en: "End",                  es: "Fin" },
            "lblDevDateTime":    { it: "Data e ora dispositivo:", en: "Device Date Time",   es: "Fecha y hora del dispositivo" }
        };


        translator.addTranslations(this.translations);
        registerTimer("scheduler", this.onTick.bind(this));
    }

    getScheduleButton(pinNumber) {
        if(!pinNumber || Number(pinNumber)<=0) return;
        const schedules = this.getSchedules();
        if(!schedules) return;
        let lblName = 'lblSchedule';
        if(schedules[pinNumber])
        {
            lblName = 'lblEdit';
        }
        return  `<button class="btn btn-sm btn-outline-secondary ms-2 schedule-btn secondFont"
                        data-pin-number="${pinNumber}" data-tr="${lblName}">
                      Programma
                  </button>
                `;
    }
    
    getHtml() {
        //const lblCancel = translator.tr("lblCancel");
        const lblScheduleType = translator.tr("lblScheduleType");
        const lblWeekly = translator.tr("lblWeekly");
        const lblOneTime = translator.tr("lblOneTime");
        const lblDevDateTime = translator.tr("lblDevDateTime");
        

        const scheduleModalHtml = `
            <div class="modal fade" id="scheduleModal" tabindex="-1">
                <div class="modal-dialog modal-dialog-centered">
                    <div class="modal-content">

                    <div class="modal-header">
                        <h5 class="modal-title"><span data-tr="lblSchedule" class='textGlow'></span><span id="lblLineName" class='textGlow'></span><span class='textGlow' id="schPin"></span></h5>
                        <button type="button" class="btn-close" data-bs-dismiss="modal"></button>
                    </div>

                    <div class="modal-body">
                        <!-- Device DeteTime -->
                        <div class="mb-3">
                            <label class="form-label KeyText text-center w-100" data-tr="lblDevDateTime">${lblDevDateTime}</label>
                            <div class="KeyText textGlowOrange text-center w-100" id="divDeviceDateTime"></div>
                        </div>
                        
                        <!-- Type -->
                        <div class="mb-3">
                            <label class="form-label" data-tr="lblScheduleType">${lblScheduleType}</label>
                            <select id="schType" class="form-select">
                                <option value="weekly">${lblWeekly}</option>
                                <option value="onetime">${lblOneTime}</option>
                            </select>
                        </div>

                        <!-- WEEKLY -->
                        <div id="schWeeklyFields">
                            <label class="form-label">Giorni</label>
                         <div class="btn-group d-block" role="group" aria-label="Select week days">
                            <input type="checkbox" class="btn-check" autocomplete="off" value="2" id="dayMon">
                            <label class="btn btn-outline-primary" for="dayMon" data-tr="lblDayMon"></label>

                            <input type="checkbox" class="btn-check" autocomplete="off" value="4" id="dayTue">
                            <label class="btn btn-outline-primary" for="dayTue" data-tr="lblDayTue"></label>

                            <input type="checkbox" class="btn-check" autocomplete="off" value="8" id="dayWed">
                            <label class="btn btn-outline-primary" for="dayWed" data-tr="lblDayWed"></label>

                            <input type="checkbox" class="btn-check" autocomplete="off" value="16" id="dayThu">
                            <label class="btn btn-outline-primary" for="dayThu" data-tr="lblDayThu"></label>

                            <input type="checkbox" class="btn-check" autocomplete="off" value="32" id="dayFri">
                            <label class="btn btn-outline-primary" for="dayFri" data-tr="lblDayFri"></label>

                            <input type="checkbox" class="btn-check" autocomplete="off" value="64" id="daySat">
                            <label class="btn btn-outline-primary" for="daySat" data-tr="lblDaySat"></label>

                            <input type="checkbox" class="btn-check" autocomplete="off" value="1" id="daySun">
                            <label class="btn btn-outline-primary" for="daySun" data-tr="lblDaySun"></label>
                        </div>

            
                            <div class="mb-3">
                                <label class="form-label" data-tr="lblTimeOn"></label>
                                <input id="schOnTime" type="time" class="form-control">
                            </div>

                            <div class="mb-3">
                                <label class="form-label" data-tr="lblTimeOff"></label>
                                <input id="schOffTime" type="time" class="form-control">
                            </div>

                        </div>

                        <!-- ONE TIME -->
                        <div id="schOneTimeFields" style="display:none;">
                        <div class="mb-3">
                            <label class="form-label" data-tr="lblDateTimeOn"></label>
                            <input id="schStartDT" type="datetime-local" class="form-control">
                        </div>

                        <div class="mb-3">
                            <label class="form-label" data-tr="lblDateTimeOff"></label>
                            <input id="schEndDT" type="datetime-local" class="form-control">
                        </div>
                        </div>

                    </div>

                    <div class="modal-footer">
                        <button class="btn btn-secondary" data-bs-dismiss="modal" data-tr="lblClose">Chiudi</button>
                        <button id="schSaveBtn" class="btn btn-primary" data-tr="lblSave"></button>
                    </div>

                    </div>
                </div>
                </div>
        `;
        

        return scheduleModalHtml;

    }

initObjects() {
    document.querySelectorAll(".schedule-btn").forEach(btn => {
        btn.addEventListener("click", () => {
            const pin = Number(btn.dataset.pinNumber);
            this.openScheduleModal(pin);
        });
    });
    //Program Change (weekly/onetime)
    document.getElementById("schType")?.addEventListener("change", e => {
        const type = e.target.value;
        document.getElementById("schWeeklyFields").style.display = type === "weekly" ? "block" : "none";
        document.getElementById("schOneTimeFields").style.display = type === "onetime" ? "block" : "none";
    });
    //Program Save
    document.getElementById("schSaveBtn")?.addEventListener("click", () => {
        this.saveSchedule();
    });

    document.addEventListener('click', (e) => {
             const btn = e.target.closest('.remove-btn');
            if (!btn) return;
            const pin = Number(btn.dataset.pin);
            //const id  = Number(btn.dataset.id);
            this.removeSchedule(pin);
    });


    this.renderSchedules();
    this.updateScheduleButtons();
}



openScheduleModal(pin) {
    this.currentPin = pin;
    //const schedules = this.getSchedules();
    //setTextContent("divDeviceDateTime", this.time.formatNow());
    const lblPin = document.getElementById("schPin");
    if(lblPin) lblPin.textContent = " (" + pin + ")";
    const lblLineName = document.getElementById("lblLineName");
    const pins = Array.isArray(this.data.pins) ? this.data.pins : [];
    const pinObj = pins.find(p => p.number === pin);
    if(pinObj && lblLineName)  lblLineName.textContent = " " + pinObj.name + " ";
    // fields reset
    document.getElementById("schType").value = "weekly";
    document.getElementById("schWeeklyFields").style.display = "block";
    document.getElementById("schOneTimeFields").style.display = "none";
    const modal = new bootstrap.Modal(document.getElementById("scheduleModal"));
    this.updateModalForm(pin);
    modal.show();
}



async saveSchedule() {
    loader(true);
    const pin = this.currentPin;
    const type = document.getElementById("schType").value;
    const form = new URLSearchParams();
    form.append("pin", pin);
    form.append("type", type);
    if (type === "weekly") {
            const mask = this.getSelectedDaysMask();
            if (mask === 0) {
                toast("Seleziona almeno un giorno", "danger");
                loader(false);
                return;
            }

            const on = this.timeToMinutes(document.getElementById("schOnTime").value);
            const off = this.timeToMinutes(document.getElementById("schOffTime").value);

            // invia solo interi validi 0-1439
            form.append("days", mask);
            form.append("on", on);
            form.append("off", off);

            // solo per debug/UI, non per il server
            //console.log(`Pin ${pin} weekly schedule: ${on} → ${off}${on > off ? " (del giorno successivo)" : ""}`);

    } else {
            const start = this.dtToUnix(document.getElementById("schStartDT").value);
            const end = this.dtToUnix(document.getElementById("schEndDT").value);
            form.append("start", start);
            form.append("end", end);

            if (!start || start <= 0 || !end || end <= 0) {
                toast("Parametri non validi", "danger");
                loader(false);
                return;
            }
    }

    try {
        const updated = await sendAPI(this.data.Name + "/schedule/add", {
            method: "POST",
            body: form.toString(),
            return: "json"
        });
        this.data.JsonConfig = JSON.stringify(updated);
        await this.removeSchAccordion(pin, 1500);
        this.renderSchedules();
        this.updateScheduleButtons();
        toast("Programma salvato", "success");
        bootstrap.Modal.getInstance(document.getElementById("scheduleModal")).hide();
        this.updatePinToggleState(Number(pin));
    } catch (err) {
        toast(err.message || err, "danger");
    } finally {
        loader(false);
    }
}

renderSchedules() {
    const schedules = this.getSchedules();
    const lblSchedule = translator.tr("lblSchedule");
    let lblType = "";
    
    Object.keys(schedules).forEach(pin => {
        const container = document.querySelector(`.relay-switch[data-pin-number="${pin}"]`);
        if (!container) return;

        // Crea ID univoco per l'accordion di questo pin
        const accId = `scheduleAccordionPin${pin}`;

        let html = `
            <div class="accordion mt-2 ms-2 schedule-accordion" id="${accId}">
        `;
      
        schedules[pin].forEach((s, idx) => {
            const itemId = `${accId}-item${idx}`;

            let onStr = "";
            let offStr = "";
            let overnightNote = "";

            if (s.type === "weekly") {
                lblType = translator.tr("lblWeekly");
                onStr = `${Math.floor(s.on/60).toString().padStart(2,'0')}:${(s.on%60).toString().padStart(2,'0')}`;
                offStr = `${Math.floor(s.off/60).toString().padStart(2,'0')}:${(s.off%60).toString().padStart(2,'0')}`;

                if (s.on > s.off) {
                    overnightNote = " " + translator.tr("lblNextDay");
                }
            }
            else
            {
                lblType = translator.tr("lblOneTime");
            }

                        
            html += `
                <div class="accordion-item">
                    <h2 class="accordion-header" id="heading-${itemId}">
                        <button class="accordion-button collapsed textGlowYellow secondFont" type="button" data-bs-toggle="collapse"
                                data-bs-target="#collapse-${itemId}" aria-expanded="false" aria-controls="collapse-${itemId}">
                                <b id="headerText-${itemId}">${lblSchedule}: (${lblType})</b>
                        </button>
                    </h2>
                    <div id="collapse-${itemId}" class="accordion-collapse collapse" aria-labelledby="heading-${itemId}" data-bs-parent="#${accId}">
                        <div class="accordion-body small">
                                <div><b data-tr="lblDevDateTime">${translator.tr("lblDevDateTime")}</b>: <div id='divDevDateTime-${pin}'></div></div>
                            ${s.type === "weekly" ? `
                                <div><b data-tr="lblDays">${translator.tr("lblDays")}</b>: ${this.formatDaysMask(s.days)}</div>
                                <div><b data-tr="lblTimeSlot">${translator.tr("lblTimeSlot")}</b>: ${this.formatWeeklySchedule(s)}</div>
                            ` : `
                                <div><b data-tr="lblStart">${translator.tr("lblStart")}</b>: ${new Date(s.start*1000).toLocaleString()}</div>
                                <div><b data-tr="lblEnd">${translator.tr("lblEnd")}</b>: ${new Date(s.end*1000).toLocaleString()}</div>
                            `}
                            <button class="btn btn-sm btn-danger mt-1 remove-btn" data-tr="lblDelete" 
                                    data-pin="${pin}"
                                    data-id="${s.id}">
                                ${translator.tr("lblDelete")}
                            </button>
                       </div>
                    </div>
                </div>
            `;
        });
        html += `</div>`; // chiude accordion
        container.parentNode.querySelectorAll('.schedule-accordion')
                .forEach(el => {
                    if (el.id === accId) el.remove();
                });

        container.insertAdjacentHTML("afterend", html);
        this.updatePinToggleState(Number(pin));

    });
}



       getSelectedDaysMask() {
            let mask = 0;
            document.querySelectorAll("#schWeeklyFields input.btn-check:checked").forEach(cb => {
                mask |= Number(cb.value);
            });
            return mask;
        }


        timeToMinutes(str) {
            const [h, m] = str.split(":").map(Number);
            return h * 60 + m;
        }

        dtToUnix(str) {
            return Math.floor(new Date(str).getTime() / 1000);
        }

        updateScheduleButtons() {
            const schedules = this.getSchedules();
            document.querySelectorAll(".schedule-btn").forEach(btn => {
                const pin = Number(btn.dataset.pinNumber);
                const hasSchedules = Array.isArray(schedules[pin]) && schedules[pin].length > 0;
                 let lblName = hasSchedules ? "lblEdit" : "lblSchedule"; 
                 btn.textContent = translator.tr(lblName);
                 btn.disabled = false;
            });
        }

          async removeSchedule(pin) {
                loader(true);
                try {
                    const form = new URLSearchParams();
                    form.append("pin", pin);
                    const updated = await sendAPI(this.data.Name + "/schedule/remove", {
                        method: "POST",
                        body: form.toString(),
                        return: "json"
                    });
                    this.data.JsonConfig = JSON.stringify(updated);
                    await this.removeSchAccordion(pin, 1500);
                    this.renderSchedules();
                    this.updateScheduleButtons();
                    toast("Programma rimosso", "success");
                } catch (err) {
                    toast(err.message || err, "danger");
                } finally {
                    loader(false);
                }

            }

            hasSchedules(pinNumber) {
                let cfg = {};
                try { cfg = JSON.parse(this.data.JsonConfig || "{}"); }
                catch { return false; }
                const schedules = cfg.schedules || {};
                return Array.isArray(schedules[pinNumber]) && schedules[pinNumber].length > 0;
            }



                formatWeeklySchedule(s) {
                    // minuti → HH:MM
                    const fmt = (min) => {
                        const h = Math.floor(min / 60).toString().padStart(2,'0');
                        const m = (min % 60).toString().padStart(2,'0');
                        return `${h}:${m}`;
                    };

                    const onStr  = fmt(s.on);
                    const offStr = fmt(s.off);

                    // overnight?
                    const overnight = s.on > s.off;

                    return overnight
                        ? `${onStr} → ${offStr} (giorno dopo)`
                        : `${onStr} → ${offStr}`;
                }


            formatDaysMask(daysMask) {
                const dayNames = ["Dom", "Lun", "Mar", "Mer", "Gio", "Ven", "Sab"];
                let result = [];
                for (let i = 0; i < 7; i++) {
                    if (daysMask & (1 << i)) result.push(dayNames[i]);
                }
                return result.join(", ");
            }


            updatePinToggleState(pinNumber) {
                const container = document.querySelector(`.relay-switch[data-pin-number="${pinNumber}"]`);
                if (!container) return;
                const toggle = container.querySelector(".relay-toggle");
                if (!toggle) return;
                const locked = this.hasSchedules(pinNumber);
                if (locked) {
                    toggle.classList.remove("on");   // forza OFF
                    toggle.classList.add("disabled");
                    toggle.style.pointerEvents = "none";
                    toggle.style.opacity = "0.4";
                } else {
                    toggle.classList.remove("disabled");
                    toggle.style.pointerEvents = "";
                    toggle.style.opacity = "";
                }
            }


            getSchedules()
            {
                let cfg = {};
                try { cfg = JSON.parse(this.data.JsonConfig || "{}"); }
                catch { cfg = {}; }
                return cfg.schedules || {};
            }


            updateModalForm(pin) {
                    pin = String(pin);
                    const schedules = this.getSchedules();

                    // reset base (così eviti sporcizia da aperture precedenti)
                    document.getElementById("schType").value = "weekly";
                    document.getElementById("schWeeklyFields").style.display = "block";
                    document.getElementById("schOneTimeFields").style.display = "none";

                    // reset giorni
                    document.querySelectorAll('#schWeeklyFields input[type="checkbox"]')
                        .forEach(cb => cb.checked = false);

                    document.getElementById("schOnTime").value = "";
                    document.getElementById("schOffTime").value = "";
                    document.getElementById("schStartDT").value = "";
                    document.getElementById("schEndDT").value = "";

                    if (!schedules[pin] || schedules[pin].length === 0)
                        return; // nessuna programmazione → modale vuota

                    // prendo il primo schedule (1 form = 1 schedule)
                    const s = schedules[pin][0];

                    const typeSel = document.getElementById("schType");
                    const weeklyFields = document.getElementById("schWeeklyFields");
                    const oneTimeFields = document.getElementById("schOneTimeFields");

                    // ========================
                    // WEEKLY
                    // ========================
                    if (s.type === "weekly") {

                        typeSel.value = "weekly";
                        weeklyFields.style.display = "block";
                        oneTimeFields.style.display = "none";

                        // giorni (bitmask)
                        document.querySelectorAll('#schWeeklyFields input[type="checkbox"]')
                            .forEach(cb => {
                                const bit = parseInt(cb.value);
                                cb.checked = (s.days & bit) !== 0;
                            });

                        // minuti → HH:MM
                        const toTime = (mins) => {
                            const h = Math.floor(mins / 60).toString().padStart(2, '0');
                            const m = (mins % 60).toString().padStart(2, '0');
                            return `${h}:${m}`;
                        };

                        document.getElementById("schOnTime").value = toTime(s.on);
                        document.getElementById("schOffTime").value = toTime(s.off);
                    }

                    // ========================
                    // ONE TIME
                    // ========================
                    else if (s.type === "onetime") {

                        typeSel.value = "onetime";
                        weeklyFields.style.display = "none";
                        oneTimeFields.style.display = "block";

                        const toLocalDT = (ts) => {
                            const d = new Date(ts * 1000);

                            const yyyy = d.getFullYear();
                            const mm = String(d.getMonth() + 1).padStart(2, '0');
                            const dd = String(d.getDate()).padStart(2, '0');
                            const hh = String(d.getHours()).padStart(2, '0');
                            const mi = String(d.getMinutes()).padStart(2, '0');

                            return `${yyyy}-${mm}-${dd}T${hh}:${mi}`;
                        };

                        document.getElementById("schStartDT").value = toLocalDT(s.start);
                        document.getElementById("schEndDT").value = toLocalDT(s.end);
                    }
                }


/*
                checkExpiredSchedules() {
                    const now = this.time.unix * 1000;
                    const schedules = this.getSchedules();
                    Object.keys(schedules).forEach(pin => {
                        const pinSchedules = schedules[pin];
                        if (!pinSchedules) return;
                        let isActive = false;
                        let removeAccordion = true;
                        const remaining = [];
                        pinSchedules.forEach(s => {
                            if (this.isScheduleActive(s, now)) {
                                this.updateToggle(pin, true, 1, "", "none");
                            }
                            else
                            {
                                this.updateToggle(pin, true, 0, "0.4", "none");
                            }
                            if (s.type === "weekly") {
                                remaining.push(s);
                                return;
                            }
                            const endMs = s.end * 1000;
                            if (endMs > now) {
                                remaining.push(s);
                                return;
                            } else {
                                this.updateToggle(pin, false, 0, "", "");
                                const btn = document.querySelector(`.schedule-btn[data-pin-number="${pin}"]`);
                                if(btn) btn.textContent = translator.tr("lblSchedule");
                                this.removeSchAccordion(pin, 1500);
                                return;
                            }
                        });
                        if (remaining.length > 0) {
                            schedules[pin] = remaining;
                        } else {
                            delete schedules[pin];
                        }
                    });
                }
*/

            checkExpiredSchedules() {
                const now = this.time.unix * 1000;
                const schedules = this.getSchedules();
                for (const pin in schedules) {
                    const pinSchedules = schedules[pin];
                    if (!pinSchedules) continue;
                    const remaining = [];
                    let shouldRemoveAccordion = false;
                    let active = false;
                    for (const s of pinSchedules) {
                        // Weekly → sempre rimanente
                        if (s.type === "weekly") {
                            remaining.push(s);
                            continue;
                        }
                        const isActive = this.isScheduleActive(s, now);
                        active = active || isActive;
                        if (isActive) {
                            this.updateToggle(pin, true, 1, "", "none");
                            remaining.push(s);
                            continue;
                        }
                        const endMs = s.end * 1000;
                        if (endMs > now) {
                            // Non attivo ma non scaduto → rimane
                            this.updateToggle(pin, true, 0, "0.4", "none");
                            remaining.push(s);
                        } else {
                            // Scaduto → rimuovere
                            shouldRemoveAccordion = true;
                        }
                    }
                    // Se qualche schedule è scaduto
                    if (shouldRemoveAccordion) {
                        this.updateToggle(pin, false, 0, "", "");
                        const btn = document.querySelector(`.schedule-btn[data-pin-number="${pin}"]`);
                        if (btn) btn.textContent = translator.tr("lblSchedule");
                        this.removeSchAccordion(pin, 1500);
                    }
                    // Aggiorna struttura
                    if (remaining.length > 0) {
                        schedules[pin] = remaining;
                    } else {
                        delete schedules[pin];
                    }
                }
            }





            isScheduleActive(s, now) {
                if (s.type === "weekly") return this.isWeeklyActive(s, now);
                if (s.type === "onetime") return this.isOneTimeActive(s, now);
                return false;
            }


            isOneTimeActive(s, now) {
                const start = s.start * 1000;
                const end   = s.end   * 1000;
                return now >= start && now < end;
            }


            isWeeklyActive(s, now) {
                const d = new Date(now);
                // giorno della settimana (0=dom, 1=lun, ...)
                const dow = d.getDay();
                // bitmask: devi mappare il tuo sistema
                // supponiamo: 1=lun, 2=mar, 4=mer, 8=g...
                const bit = 1 << (dow - 1);  
                if (dow === 0) return false; // domenica fuori bitmask? dipende da te
                if ((s.days & bit) === 0) return false;
                // minuti attuali
                const minsNow = d.getHours() * 60 + d.getMinutes();
                return minsNow >= s.on && minsNow < s.off;
            }


            async removeSchAccordion(pin, time = 500) {
                const el = document.getElementById(`scheduleAccordionPin${pin}`);
                if (!el) return;
                this.fadeOutAndRemove(el, time);
                await new Promise(resolve => setTimeout(resolve, time));
            }


            updateToggle(pinNumber, disabled, status, opacity, pointerEvents) {
                const container = document.querySelector(`.relay-switch[data-pin-number="${pinNumber}"]`);
                if (!container) return;
                const toggle = container.querySelector(".relay-toggle");
                if (!toggle) return;
                toggle.classList.toggle("disabled", disabled);
                toggle.classList.toggle("on", status === 1);
                toggle.classList.toggle("off", status === 0);
                if (opacity != null) toggle.style.opacity = opacity;
                if (pointerEvents != null) toggle.style.pointerEvents = pointerEvents;
            }

            
            fadeOutAndRemove(el, duration = 300) {
                if (!el) return;
                el.style.transition = `opacity ${duration}ms ease`;
                el.style.opacity = 0;
                setTimeout(() => el.remove(), duration);
            }

              refreshSelectType() {
                    const select = document.getElementById("schType");
                    if (!select) return;
                    const lblScheduleType = translator.tr("lblScheduleType");
                    const lblWeekly = translator.tr("lblWeekly");
                    const lblOneTime = translator.tr("lblOneTime");
                    // options
                    select.querySelector('option[value="weekly"]').textContent = lblWeekly;
                    select.querySelector('option[value="onetime"]').textContent = lblOneTime;
                }


                //LoopMaxCore change language:
                onLanguageChange(lng) {

                    this.refreshSelectType();
                    const lblSchedule = translator.tr("lblSchedule");
                    const schedules = this.getSchedules();
                    Object.keys(schedules).forEach(pin => {
                        const accId = `scheduleAccordionPin${pin}`;
                        schedules[pin].forEach((s, idx) => {
                            let lblType = "Unknown type";
                            const itemId = `headerText-${accId}-item${idx}`;
                            const currHeader = document.getElementById(itemId);
                                if(currHeader)
                                {
                                    if (s.type === "weekly") lblType = translator.tr("lblWeekly");
                                    if (s.type !== "weekly") lblType = translator.tr("lblOneTime");
                                    currHeader.textContent = `${lblSchedule}: (${lblType})`;
                                }
                        });
                    
                    });

                 }

                //LoopMaxCore Global Timer (tick every second):
                onTick(tick) {
                    // every 10 sec
                    //if (tick % 10 === 0) { this.checkExpiredSchedules(); }
                    this.checkExpiredSchedules();

                    setTextContent("divDeviceDateTime", this.time.formatNow());

                    //Refresh Eventually Dev Time into Scheduler accordions:
                    const schedules = this.getSchedules();
                    Object.keys(schedules).forEach(pin => {
                        const el = document.getElementById(`divDevDateTime-${pin}`)
                        if(!el) return;
                        el.textContent = this.time.formatNow();
                    });
                    
                }


}
