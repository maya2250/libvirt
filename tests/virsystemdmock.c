/*
 * Copyright (C) 2013 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#ifdef __linux__
# include "internal.h"

# include <stdlib.h>

# include <dbus/dbus.h>

void dbus_connection_set_change_sigpipe(dbus_bool_t will_modify_sigpipe ATTRIBUTE_UNUSED)
{
}

dbus_bool_t dbus_threads_init_default(void)
{
    return 1;
}

DBusConnection *dbus_bus_get(DBusBusType type ATTRIBUTE_UNUSED,
                             DBusError *error ATTRIBUTE_UNUSED)
{
    return (DBusConnection *)0x1;
}

void dbus_connection_set_exit_on_disconnect(DBusConnection *connection ATTRIBUTE_UNUSED,
                                            dbus_bool_t exit_on_disconnect ATTRIBUTE_UNUSED)
{
}


dbus_bool_t dbus_connection_set_watch_functions(DBusConnection *connection ATTRIBUTE_UNUSED,
                                                DBusAddWatchFunction add_function ATTRIBUTE_UNUSED,
                                                DBusRemoveWatchFunction remove_function ATTRIBUTE_UNUSED,
                                                DBusWatchToggledFunction toggled_function ATTRIBUTE_UNUSED,
                                                void *data ATTRIBUTE_UNUSED,
                                                DBusFreeFunction free_data_function ATTRIBUTE_UNUSED)
{
    return 1;
}

dbus_bool_t dbus_message_set_reply_serial(DBusMessage *message ATTRIBUTE_UNUSED,
                                          dbus_uint32_t serial ATTRIBUTE_UNUSED)
{
    return 1;
}

DBusMessage *dbus_connection_send_with_reply_and_block(DBusConnection *connection ATTRIBUTE_UNUSED,
                                                       DBusMessage *message ATTRIBUTE_UNUSED,
                                                       int timeout_milliseconds ATTRIBUTE_UNUSED,
                                                       DBusError *error)
{
    DBusMessage *reply = NULL;

    if (getenv("FAIL_BAD_SERVICE")) {
        DBusMessageIter iter;
        const char *error_message = "Something went wrong creating the machine";
        if (!(reply = dbus_message_new(DBUS_MESSAGE_TYPE_ERROR)))
            return NULL;
        dbus_message_set_error_name(reply, "org.freedesktop.systemd.badthing");
        dbus_message_iter_init_append(reply, &iter);
        if (!dbus_message_iter_append_basic(&iter,
                                            DBUS_TYPE_STRING,
                                            &error_message)) {
            dbus_message_unref(reply);
            return NULL;
        }
    } else if (getenv("FAIL_NO_SERVICE")) {
        dbus_set_error(error,
                       "org.freedesktop.DBus.Error.ServiceUnknown",
                       "%s", "The name org.freedesktop.machine1 was not provided by any .service files");
    } else {
        reply = dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_RETURN);
    }

    return reply;
}

#else
/* Nothing to override on non-__linux__ platforms */
#endif
