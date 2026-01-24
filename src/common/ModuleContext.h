#pragma once
#include <vector>

namespace LoopMax::Core {
    class ServicesManager;
    class IService;
}

namespace LoopMax::Services {

    class ITimer;
    class ILogs;
    class ISerial;
    class ISystem;
    class IPins;
    class IReset;
    class IGeoInfo;
    class IUpdate;
    class IConfig;
    class IWifi;
    class IWeb;
    class IHttps;
    class IMqtts;

    class ModuleContext {
    public:
        ModuleContext(
            LoopMax::Core::ServicesManager& srv,
            ITimer &t,
            ILogs &l,
            ISerial &s,
            ISystem &sy,
            IPins &p,
            IReset &r,
            IGeoInfo &g,
            IUpdate &u,
            IConfig &c,
            IWifi &w,
            IWeb &we,
            IHttps &htt,
            IMqtts &mqt
        );

        const std::vector<LoopMax::Core::IService*>& Services() const;

        // (opzionale) getter se ti servono fuori
        ITimer &time;
        ILogs &logs;
        ISerial &serial;
        ISystem &sys;
        IPins &pins;
        IReset &reset;
        IGeoInfo &geo;
        IUpdate &update;
        IConfig &config;
        IWifi &wifi;
        IWeb &web;
        IHttps &https;
        IMqtts &mqtts;

    private:
        LoopMax::Core::ServicesManager &srvMan;

        
    };
}
