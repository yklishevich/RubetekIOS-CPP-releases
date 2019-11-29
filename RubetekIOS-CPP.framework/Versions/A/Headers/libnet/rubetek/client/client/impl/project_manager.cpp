#include <list>
#include <string>
#include <fstream>

#include <boost/filesystem.hpp>

#include <msw/range.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/lexical_cast/bool2str.hpp>

#include <lama/libzip/unpack_all.hpp>
#include <lama/boost/include/property_tree/info_parser.hpp>

#ifndef RUBETEK_HEADER_ONLY
#   include <rubetek/client/client/project_manager.hpp>
#endif


namespace rubetek {
namespace client  {

    template <typename ProjectId>
    project_manager<ProjectId>::project_manager(std::string const& project_path)
        : logger_            ( "project manager"              )
        , project_path_      ( project_path                   )
        , projects_filename_ ( project_path + "projects.info" )
    {
        logger_.info("path: ", project_path);
        boost::filesystem::create_directories(project_path_);
        load_projects();
    }

    template <typename ProjectId>
    void project_manager<ProjectId>::add_project(project_id id, bool demo_mode)
    {
        logger_.info("add project: ", id, ", demo mode: ", msw::yes_no(demo_mode));
        if (has_project(id)) msw::throw_runtime_error("project_manager: project [", id, "] already exist");
        projects_[id].demo_mode = demo_mode;
        save_projects();
    }
    template <typename ProjectId>
    void project_manager<ProjectId>::remove_project(project_id id)
    {
        logger_.info("remove project: ", id);
        std::string const project_directory = buffer_path(id);
        boost::filesystem::remove_all(project_directory);
        projects_.erase(project_iterator(id));
        save_projects();
    }

    template <typename ProjectId>
    std::list<ProjectId> project_manager<ProjectId>::projects_id() const
    {
        std::list<ProjectId> list;
        for (auto const& pr : projects_)
            list.push_back(pr.first);
        return std::move(list);
    }
    template <typename ProjectId>
    bool project_manager<ProjectId>::has_project(project_id id) const
    {
        return projects_.count(id) > 0;
    }
    template <typename ProjectId>
    bool project_manager<ProjectId>::has_projects() const
    {
        return projects_.size() != 0;
    }

    template <typename ProjectId>
    std::string project_manager<ProjectId>::alias(project_id id) const
    {
        return get_project_info(id).alias;
    }
    template <typename ProjectId>
    void project_manager<ProjectId>::add_alias(project_id id, std::string const& alias)
    {
        logger_.info("add alias for: ", id, ": ", alias);
        project_iterator(id)->second.alias = alias;
        save_projects();
    }
    template <typename ProjectId>
    void project_manager<ProjectId>::remove_alias(project_id id)
    {
        logger_.info("remove alias for: ", id);
        project_iterator(id)->second.alias.clear();
        save_projects();
    }

    template <typename ProjectId>
    void project_manager<ProjectId>::update_name(project_id id, std::string const& name)
    {
        logger_.info("update name for: ", id, ": ", name);
        if (project_iterator(id)->second.name != name)
        {
            project_iterator(id)->second.name = name;
            save_projects();
        }
    }
    template <typename ProjectId>
    void project_manager<ProjectId>::update_project(project_id id, unsigned crc, msw::range<msw::byte const> buf)
    {
        logger_.info("update project: ", id);
        project_iterator(id)->second.project_crc = crc;
        save_projects();
        save_buffer(id, "project", buf);
    }

    template <typename ProjectId>
    typename project_manager<ProjectId>::project_files project_manager<ProjectId>::get_project_files(project_id id) const
    {
        project_iterator(id);
        project_files pf{buffer_file(id, "project")};
        return pf;
    }
    template <typename ProjectId>
    typename project_manager<ProjectId>::project_info project_manager<ProjectId>::get_project_info(project_id id) const
    {
        return project_iterator(id)->second;
    }

    template <typename ProjectId>
    void project_manager<ProjectId>::clear()
    {
        logger_.info("clear");
        auto const proj_id_list = projects_id();
        for (auto const& prj_id : proj_id_list)
            remove_project(prj_id);
    }

    template <typename ProjectId>
    typename project_manager<ProjectId>::iterator project_manager<ProjectId>::project_iterator(project_id id)
    {
        iterator it = projects_.find(id);
        if (it == projects_.end()) if (!has_project(id)) msw::throw_runtime_error("project_manager: project [", id, "] not found");
        return it;
    }
    template <typename ProjectId>
    typename project_manager<ProjectId>::const_iterator project_manager<ProjectId>::project_iterator(project_id id) const
    {
        const_iterator it = projects_.find(id);
        if (it == projects_.end()) if (!has_project(id)) msw::throw_runtime_error("project_manager: project [", id, "] not found");
        return it;
    }

    template <typename ProjectId>
    void project_manager<ProjectId>::load_projects()
    {
        logger_.info("load projects");
        boost::property_tree::ptree pt;
        std::ifstream projects_file(projects_filename_);
        boost::property_tree::read_info(projects_file, pt);
        for (auto const& prj_info : pt)
        {
            boost::property_tree::ptree const& child = prj_info.second;
            project_info& info = projects_[prj_info.first];
            info.project_crc = child.get<unsigned>    ("project_crc" , 0  );
            info.alias       = child.get<std::string> ("alias"       , "" );
            info.name        = child.get<std::string> ("name"        , "" );
            MSW_ASSERT(!info.demo_mode);
            logger_.debug("loaded: ", prj_info.first);
        }
    }
    template <typename ProjectId>
    void project_manager<ProjectId>::save_projects() const
    {
        logger_.info("save projects");
        boost::property_tree::ptree pt;
        for (auto const& info : projects_)
        {
            if (!info.second.demo_mode)
            {
                logger_.debug("save: ", info.first);
                boost::property_tree::ptree child;
                child.add("project_crc" , info.second.project_crc );
                child.add("alias"       , info.second.alias       );
                child.add("name"        , info.second.name        );
                pt.add_child(info.first, child);
            }
            else logger_.debug("not save: ", info.first, ", becouse it is demo");
        }
        std::ofstream projects_file(projects_filename_);
        boost::property_tree::write_info(projects_file, pt);
    }

    template <typename ProjectId>
    std::string project_manager<ProjectId>::buffer_path(project_id id) const
    {
        return project_path_ + boost::lexical_cast<std::string>(id) + "/";
    }
    template <typename ProjectId>
    std::string project_manager<ProjectId>::buffer_file(project_id id, std::string const& name) const
    {
        return buffer_path(id) + name + ".json";
    }
    template <typename ProjectId>
    std::string project_manager<ProjectId>::data_path(project_id id, std::string const& name) const
    {
        return buffer_path(id) + name + "/";
    }
    template <typename ProjectId>
    void project_manager<ProjectId>::save_buffer(std::string const& filename, msw::range<msw::byte const> buf) const
    {
        std::ofstream ofile(filename.c_str(), std::ios::binary);
        ofile.write(reinterpret_cast<char const*>(buf.data().native()), buf.size().count());
    }
    template <typename ProjectId>
    void project_manager<ProjectId>::save_buffer(project_id id, std::string const& name, msw::range<msw::byte const> buf) const
    {
        boost::filesystem::create_directory(buffer_path(id));
        std::string const file_name = buffer_file(id, name);
        save_buffer(file_name, buf);
    }

}}
