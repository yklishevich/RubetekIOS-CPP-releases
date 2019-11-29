#pragma once

#include <list>
#include <string>
#include <unordered_map>

#include <msw/range.hpp>
#include <msw/noncopyable.hpp>

#include <rubetek/log.hpp>
#include <rubetek/config.hpp>


namespace rubetek {
namespace client  {

    template <typename ProjectId>
    struct project_manager
        : msw::noncopyable
    {
        typedef ProjectId project_id;

        struct project_info
        {
            unsigned    project_crc ;
            std::string alias       ;
            std::string name        ;
            bool        demo_mode   ;

            project_info()
                : project_crc ( 0     )
                , demo_mode   ( false )
            {}
        };
        struct project_files
        {
            std::string project;
        };

        explicit              project_manager   (std::string const& projects_path)                                      ;

        void                  add_project       (project_id, bool demo_mode)                                            ;
        void                  remove_project    (project_id)                                                            ;

        std::list<project_id> projects_id       ()                                                                const ;
        bool                  has_project       (project_id)                                                      const ;
        bool                  has_projects      ()                                                                const ;

        std::string           alias             (project_id)                                                      const ;
        void                  add_alias         (project_id, std::string const&)                                        ;
        void                  remove_alias      (project_id)                                                            ;

        void                  update_name       (project_id, std::string const&)                                        ;
        void                  update_project    (project_id, unsigned crc, msw::range<msw::byte const>)                 ;

        project_files         get_project_files (project_id)                                                      const ;
        project_info          get_project_info  (project_id)                                                      const ;

        void                  clear             ()                                                                      ;

    private:
        typedef          std::unordered_map<project_id, project_info> projects       ;
        typedef typename projects::iterator                           iterator       ;
        typedef typename projects::const_iterator                     const_iterator ;

        iterator              project_iterator  (project_id)                                                             ;
        const_iterator        project_iterator  (project_id)                                                       const ;

        void                  load_projects     ()                                                                       ;
        void                  save_projects     ()                                                                 const ;

        std::string           buffer_path       (project_id)                                                       const ;
        std::string           buffer_file       (project_id, std::string const& name)                              const ;
        std::string           data_path         (project_id, std::string const& name)                              const ;

        void                  save_buffer       (std::string const& filename, msw::range<msw::byte const>)         const ;
        void                  save_buffer       (project_id, std::string const& name, msw::range<msw::byte const>) const ;

        logger      const logger_            ;
        std::string const project_path_      ;
        std::string const projects_filename_ ;
        projects          projects_          ;
    };

}}


#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/client/client/impl/project_manager.cpp>
#endif
