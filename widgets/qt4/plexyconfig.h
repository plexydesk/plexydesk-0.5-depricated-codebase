
#ifndef PLEXY_CONIFG_LINUX_QT_H
#define PLEXY_CONIFG_LINUX_QT_H

namespace PlexyDesk
{
    class Config
    {
        public:
           static Config*  getInstance();
           void read();
           void save();

        private:
           static Config * config;
           Config(){};
           Config(Config &){}
           Config& operator=(const Config&);
    };
} // namespace PlexyDesk



#endif
