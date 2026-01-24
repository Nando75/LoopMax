
<p align="center">
  <img src="docs/img/LoopMaxOS.webp" alt="LoopMax Embedded OS" width="600">
</p>

<p align="center">
  Modular Embedded Operating System<br>
  Core • Web OS • HAL • Dynamic Modules
</p>

---

## 🔍 What is LoopMax

**LoopMax Embedded OS** is a modular framework for embedded systems designed
to provide a **shared core**, independent from hardware and framework,
on top of which complete devices can be built using
**dynamically loaded application modules**.

LoopMax includes a **Web-Based Operating System** and all essential system services,
allowing developers to focus only on application logic.

---

## 🚀 Why LoopMax

Because enough with:

- scattered projects
- duplicated base code
- reinvented web interfaces
- repeated system services on every device

With LoopMax:

- you develop **only your module**
- the **core** is already available
- the **Web OS** is integrated
- the infrastructure is shared
- the device is immediately operational

👉 One system  
👉 One core  
👉 Infinite devices  
👉 Same professional UI Bootstap.


---

## 🧩 Architecture (overview)

LoopMax is structured in multiple layers:

- **LoopMax Core**
  - Web-Based OS
  - System services
  - Module management
- **HAL (Hardware Abstraction Layer)**
  - Multi-framework support (Arduino / ESP-IDF)
  - Multi-board support
- **Application Modules**
  - Zappy2 (free 2 channels rele module)
  
  - Extensible over time

---
## 🚀 HAL (Hardware Abstraction Layer)
## Service Manager Architecture

Each Service Manager extends three interfaces:

- **`IService`**  
  Used by the core to manage the service lifecycle.

- **`IHal_XxxxManager`**  
  Core manager interface, with a base class that varies depending on the underlying hardware.

- **`IXxxx`**  
  Public interface exposed to external application modules and implemented by the various service managers.

Below is an example of the structure of a single service:
<p align="center">
  <img src="docs/img/Hal.webp" alt="LoopMax Embedded OS" width="600">
</p>



---

## 🧩 Web UI (Bootstrap)

<h5 align="center">Relè module (2ch)</h5>
<p align="center">
  <img src="docs/img/ui_Module.webp" alt="LoopMax Embedded OS" width="600">
</p>

<h5 align="center">Device configuration</h5>
<p align="center">
  <img src="docs/img/ui_Configuration.webp" alt="LoopMax Embedded OS" width="600">
</p>

<h5 align="center">Application Modules</h5>
<p align="center">
  <img src="docs/img/ui_ModuleList.webp" alt="LoopMax Embedded OS" width="600">
</p>

<h5 align="center">Device services</h5>
<p align="center">
  <img src="docs/img/ui_Services.webp" alt="LoopMax Embedded OS" width="600">
</p>

<h5 align="center">Service: System</h5>
<p align="center">
  <img src="docs/img/ui_System.webp" alt="LoopMax Embedded OS" width="600">
</p>

<h5 align="center">Service: Wifi</h5>
<p align="center">
  <img src="docs/img/ui_Wifi.webp" alt="LoopMax Embedded OS" width="600">
</p>

<h5 align="center">Service: Web</h5>
<p align="center">
  <img src="docs/img/ui_Web.webp" alt="LoopMax Embedded OS" width="600">
</p>

<h5 align="center">Boot timeline</h5>
<p align="center">
  <img src="docs/img/ui_BootTimeLine.webp" alt="LoopMax Embedded OS" width="600">
</p>

<h5 align="center">Events timeline</h5>
<p align="center">
  <img src="docs/img/EventsTimeline.webp" alt="LoopMax Embedded OS" width="600">
</p>

<h5 align="center">System Logs</h5>
<p align="center">
  <img src="docs/img/ui_Logs.webp" alt="LoopMax Embedded OS" width="600">
</p>



---

## 🧠 Philosophy

LoopMax was created to eliminate embedded project fragmentation
and maximize code reuse.

Every new feature is a **module**,
not a new project from scratch.

---

## ⚠️ License

This project is available **for personal, non-commercial use only**.  
See the `LICENSE` file for full terms.

---

## 🚧 Project Status

The project is under active development.  
Technical documentation will be expanded progressively.
