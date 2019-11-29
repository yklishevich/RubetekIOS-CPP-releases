#pragma once

#include <msw/entry_point/classic.hpp>

#include <lama/qt/include/message_box.hpp>
#include <lama/qt/include/application.hpp>


namespace lama {
namespace qt   {

    struct app_ctx
    {
        app_ctx
        (
              int           ac_
            , char**        av_
            , QApplication& app
        )
            : ac   ( ac_ )
            , av   ( av_ )
            , app_ ( app )
        {}

        int    const ac ;
        char** const av ;

        void exec()
        {
            app_.exec();
        }
    private:
        QApplication& app_ ;
    };

    void app_exec(app_ctx);

}}


void msw::main(int ac, char** av)
{
    QApplication app(ac, av);
    try
    {
        lama::qt::app_exec(lama::qt::app_ctx(ac, av, app));
    }
    catch (std::exception const& e)
    {
        QMessageBox msg_box(QMessageBox::Critical, "Exception", e.what());
        msg_box.exec();
    }
    catch (...)
    {
        QMessageBox msg_box(QMessageBox::Critical, "Exception", "unknown error");
        msg_box.exec();
    }
}
