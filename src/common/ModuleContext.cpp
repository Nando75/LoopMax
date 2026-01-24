#include "ModuleContext.h"
#include "core/ServicesManager.h"

namespace LoopMax::Services {

    ModuleContext::ModuleContext(
        LoopMax::Core::ServicesManager &srv,
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

    )
        : srvMan(srv),
          time(t),
          logs(l),
          serial(s),
          sys(sy),
          pins(p),
          reset(r),
          geo(g),
          update(u),
          config(c),
          wifi(w),
          web(we),
          https(htt),
          mqtts(mqt)
    {}

    const std::vector<LoopMax::Core::IService*>& ModuleContext::Services() const {
        return srvMan.getServices();
    }
}
