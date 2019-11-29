#pragma once

#include <list>
#include <string>

#include <boost/optional.hpp>
#include <boost/filesystem.hpp>

#include <msw/normilize_path.hpp>


namespace msw {
namespace zzz {

    template <typename Iterator>
    std::list<boost::filesystem::path> directory_content(boost::filesystem::path const& root)
    {
        std::list<boost::filesystem::path> result;
        if(boost::filesystem::exists(root) || boost::filesystem::is_directory(root))
        {
            Iterator i(root);
            Iterator endi;
            for (; i != endi; ++i) result.push_back(i->path());
        }
        return std::move(result);
    }

    inline bool directory_content_check_filename(boost::filesystem::path const& path, boost::optional<std::string> extension)
    {
        if (boost::filesystem::is_regular_file(path))
        {
            if (extension) return path.has_extension() && (*extension == path.extension());
            else return true;
        }
        return false;
    }

}}


namespace msw
{
    inline std::list<boost::filesystem::path> directory_content(boost::filesystem::path const& root)
    {
        return zzz::directory_content<boost::filesystem::directory_iterator>(root);
    }
    inline std::list<boost::filesystem::path> directory_recursive_content(boost::filesystem::path const& root)
    {
        return zzz::directory_content<boost::filesystem::recursive_directory_iterator>(root);
    }
    inline std::list<boost::filesystem::path> directory_recursive_relative_content(std::string const& directory)
    {
        boost::filesystem::path const root = normilize_path(directory);
        using namespace boost::filesystem;
        std::list<path> files = zzz::directory_content<recursive_directory_iterator>(root);
        std::size_t const root_size = root.string().size();
        for (path& p : files)
        {
            std::string s = p.string();
            p = s.substr(root_size - 1);
        }
        return std::move(files);
    }

    inline std::list<boost::filesystem::path> directory_files(boost::filesystem::path const& root, boost::optional<std::string> extension = boost::none)
    {
        std::list<boost::filesystem::path> lst = directory_content(root);
        lst.remove_if
        (
            [&extension](boost::filesystem::path const& path) -> bool
            {
                return !zzz::directory_content_check_filename(path, extension);
            }
        );
        return std::move(lst);
    }
    inline std::list<boost::filesystem::path> directory_files(boost::filesystem::path const& root, std::string extension)
    {
        return directory_files(root, boost::optional<std::string>(extension));
    }
}
