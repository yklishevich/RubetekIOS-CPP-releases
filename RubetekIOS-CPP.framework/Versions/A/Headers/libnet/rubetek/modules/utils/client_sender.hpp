#ifndef CC_CLIENT_SENDER_HPP
#define CC_CLIENT_SENDER_HPP

#include <string>

#include <rubetek/module/context2.hpp>
#include <rubetek/proto/thrift/serialize.hpp>

namespace rubetek
{
    namespace module
    {
        class client_sender
        {
          public:

            typedef std::function<std::string(tag_type command)> t_get_command_name;

            client_sender(context2::ptr ctx, t_get_command_name get_command_name_fn);

            void send_client_response(tag_type command, tag_type search_command, const std::string &payload, tag_type replace_command = 0);
            void send_all_online_clients(tag_type command, const std::string &payload, tag_type search_command);
            void add_command(const std::string &client_id, tag_type command);
            void remove_command(tag_type command);

          private:

            typedef std::pair<std::string, tag_type>            t_incoming_commands;
            typedef std::list<t_incoming_commands>::iterator    t_command_it;

            std::list<t_incoming_commands>  incoming_commands_;
            context2::ptr                   ctx_;
            t_get_command_name              get_command_name_fn_;
            std::list<std::string>          online_clients_;

            t_command_it get_client_id_by_command(tag_type command);
        };

        client_sender::client_sender(context2::ptr ctx, t_get_command_name get_command_name_fn)
            : ctx_(ctx)
            , get_command_name_fn_(get_command_name_fn)
        {
            if (get_command_name_fn_ == nullptr)
            {
                get_command_name_fn_ = [](tag_type command) -> std::string
                {
                    return std::to_string(command);
                };
            }

            ctx_->subscribe_s
                (
                    proto::imc::tag::client_list
                    , [this](const std::string & packet)
                    {
                        proto::imc::client_list cl_list;
                        thrift_deserialize(packet, cl_list);

                        online_clients_.clear();

                        for (auto &cl : cl_list.clients)
                        {
                            if (cl.online)
                            {
                                online_clients_.push_back(cl.id);
                            }
                        }
                    }
                );

            ctx_->send(proto::imc::tag::get_client_list);
        }

        void client_sender::send_client_response(tag_type command, tag_type search_command, const std::string &payload, tag_type replace_command)
        {
            t_command_it it = get_client_id_by_command(search_command);

            if (it != incoming_commands_.end())
            {
                proto::imc::client_command cmd;
                proto::imc::client_payload pld;

                pld.tag = command;
                pld.payload = payload;

                cmd.client_id = it->first;
                cmd.payload = thrift_serialize(pld);

                log::debug("send to client: " + it->first + " response: " + get_command_name_fn_(command));
                ctx_->send(proto::imc::tag::command_to_client, thrift_serialize(cmd));

                if (replace_command != 0)
                {
                    log::debug("replace command " + get_command_name_fn_(command) + " to command " + get_command_name_fn_(command) + " for client " + it->first);

                    it->second = replace_command;
                } else
                {
                    log::debug("remove command " + get_command_name_fn_(command) + " for client " + it->first);
                    incoming_commands_.erase(it);
                }
            } else
            {
                log::debug("warning: not found response client id for command " + get_command_name_fn_(command));
            }
        }

        void client_sender::send_all_online_clients(tag_type command, const std::string &payload, tag_type search_command)
        {
            t_command_it it = get_client_id_by_command(search_command);
            if (it != incoming_commands_.end())
            {
                log::debug("remove command " + get_command_name_fn_(command) + " for client " + it->first);
                incoming_commands_.erase(it);
            }

            for (auto &cl : online_clients_)
            {
                proto::imc::client_command cmd;
                proto::imc::client_payload pld;

                pld.tag = command;
                pld.payload = payload;

                cmd.client_id = cl;
                cmd.payload = thrift_serialize(pld);

                log::debug("send to client (online): " + cl + " response: " + get_command_name_fn_(command));
                ctx_->send(proto::imc::tag::command_to_client, thrift_serialize(cmd));
            }
        }

        client_sender::t_command_it client_sender::get_client_id_by_command(tag_type command)
        {
            for (auto it = incoming_commands_.begin(); it != incoming_commands_.end(); ++it)
            {
                if (it->second == command)
                {
                    return it;
                }
            }
            return incoming_commands_.end();
        }

        void client_sender::add_command(const std::string &client_id, tag_type command)
        {
            incoming_commands_.push_back(std::make_pair(client_id, command));
        }

        void client_sender::remove_command(tag_type command)
        {
            t_command_it it = incoming_commands_.begin();

            while (it != incoming_commands_.end())
            {
                if (it->second == command)
                {
                    log::debug("clean command " + get_command_name_fn_(command) + " for client " + it->first);
                    incoming_commands_.erase(it++);
                } else
                {
                    ++it;
                }
            }
        }
    }
}

#endif //CC_CLIENT_SENDER_HPP
