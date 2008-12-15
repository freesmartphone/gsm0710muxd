/*
valac --vapidir=src --pkg=gsm0710muxd --pkg=dbus-glib-1 --ccode src/muxercontrol.vala

DB="dbus-send --system --print-reply --type=method_call --dest=org.pyneo.mux /org/pyneo/Muxer"
$DB org.pyneo.Powered.GetPower string:xxx
$DB org.pyneo.Powered.SetPower string:xxx boolean:true
$DB org.pyneo.Powered.GetDevice string:xxx
*/

#include "muxercontrol.h"
#include <dbus/dbus-glib-lowlevel.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib.h>




static gboolean _dbus_powered_SetPower (Powered* self, const char* purpose, gboolean on, GError** error);
static gboolean _dbus_powered_GetPower (Powered* self, const char* purpose, gboolean* on, GError** error);
static gboolean _dbus_powered_GetDevice (Powered* self, const char* purpose, char** url, GError** error);
enum  {
	MUXER_DUMMY_PROPERTY
};
static Muxer* muxer_server;
static void muxer_real_SetPower (Powered* base, const char* purpose, gboolean on, GError** error);
static void muxer_real_GetPower (Powered* base, const char* purpose, gboolean* on, GError** error);
static void muxer_real_GetDevice (Powered* base, const char* purpose, char** url, GError** error);
guint _dynamic_RequestName0 (DBusGProxy* obj, const char* param1, guint param2, GError** error);
static gpointer muxer_parent_class = NULL;
static PoweredIface* muxer_powered_parent_iface = NULL;
static void muxer_dispose (GObject * obj);


static void g_cclosure_user_marshal_BOOLEAN__STRING_BOOLEAN_POINTER (GClosure * closure, GValue * return_value, guint n_param_values, const GValue * param_values, gpointer invocation_hint, gpointer marshal_data);
static void g_cclosure_user_marshal_BOOLEAN__STRING_POINTER_POINTER (GClosure * closure, GValue * return_value, guint n_param_values, const GValue * param_values, gpointer invocation_hint, gpointer marshal_data);

GQuark muxer_error_quark (void) {
	return g_quark_from_static_string ("muxer_error-quark");
}


/* switch power on/off. */
void powered_SetPower (Powered* self, const char* purpose, gboolean on, GError** error) {
	POWERED_GET_INTERFACE (self)->SetPower (self, purpose, on, error);
}


/* return if power is already on/off. */
void powered_GetPower (Powered* self, const char* purpose, gboolean* on, GError** error) {
	POWERED_GET_INTERFACE (self)->GetPower (self, purpose, on, error);
}


/* TODO upercase cores! rename it to Power() if that is fixed
 */
void powered_GetDevice (Powered* self, const char* purpose, char** url, GError** error) {
	POWERED_GET_INTERFACE (self)->GetDevice (self, purpose, url, error);
}


static gboolean _dbus_powered_SetPower (Powered* self, const char* purpose, gboolean on, GError** error) {
	powered_SetPower (self, purpose, on, error);
	return !error || !*error;
}


static gboolean _dbus_powered_GetPower (Powered* self, const char* purpose, gboolean* on, GError** error) {
	powered_GetPower (self, purpose, on, error);
	return !error || !*error;
}


static gboolean _dbus_powered_GetDevice (Powered* self, const char* purpose, char** url, GError** error) {
	powered_GetDevice (self, purpose, url, error);
	return !error || !*error;
}


static void powered_base_init (PoweredIface * iface) {
	static gboolean initialized = FALSE;
	if (!initialized) {
		initialized = TRUE;
		g_signal_new ("power", TYPE_POWERED, G_SIGNAL_RUN_LAST, 0, NULL, NULL, g_cclosure_marshal_VOID__BOOLEAN, G_TYPE_NONE, 1, G_TYPE_BOOLEAN);
		static const DBusGMethodInfo powered_dbus_methods[] = {
{ (GCallback) _dbus_powered_SetPower, g_cclosure_user_marshal_BOOLEAN__STRING_BOOLEAN_POINTER, 0 },
{ (GCallback) _dbus_powered_GetPower, g_cclosure_user_marshal_BOOLEAN__STRING_POINTER_POINTER, 49 },
{ (GCallback) _dbus_powered_GetDevice, g_cclosure_user_marshal_BOOLEAN__STRING_POINTER_POINTER, 102 },
}
;
		static const DBusGObjectInfo powered_dbus_object_info = { 0, powered_dbus_methods, 3, "org.pyneo.Powered\0SetPower\0S\0purpose\0I\0s\0on\0I\0b\0\0org.pyneo.Powered\0GetPower\0S\0purpose\0I\0s\0on\0O\0F\0N\0b\0\0org.pyneo.Powered\0GetDevice\0S\0purpose\0I\0s\0url\0O\0F\0N\0s\0\0", "org.pyneo.Powered\0power\0", "" };
		dbus_g_object_type_install_info (TYPE_POWERED, &powered_dbus_object_info);
	}
}


GType powered_get_type (void) {
	static GType powered_type_id = 0;
	if (G_UNLIKELY (powered_type_id == 0)) {
		static const GTypeInfo g_define_type_info = { sizeof (PoweredIface), (GBaseInitFunc) powered_base_init, (GBaseFinalizeFunc) NULL, (GClassInitFunc) NULL, (GClassFinalizeFunc) NULL, NULL, 0, 0, (GInstanceInitFunc) NULL };
		powered_type_id = g_type_register_static (G_TYPE_INTERFACE, "Powered", &g_define_type_info, 0);
		g_type_interface_add_prerequisite (powered_type_id, G_TYPE_OBJECT);
	}
	return powered_type_id;
}


static void muxer_real_SetPower (Powered* base, const char* purpose, gboolean on, GError** error) {
	Muxer * self;
	self = MUXER (base);
	g_return_if_fail (purpose != NULL);
	g_message ("muxercontrol.vala:30: %s: %s %d", "SetPower", purpose, on);
	c_set_power (purpose, on);
}


static void muxer_real_GetPower (Powered* base, const char* purpose, gboolean* on, GError** error) {
	Muxer * self;
	self = MUXER (base);
	g_return_if_fail (purpose != NULL);
	(*on) = c_get_power (purpose);
	g_message ("muxercontrol.vala:35: %s: %s %d", "GetPower", purpose, (*on));
}


static void muxer_real_GetDevice (Powered* base, const char* purpose, char** url, GError** error) {
	Muxer * self;
	char* fn;
	const char* _tmp3;
	char* _tmp2;
	gint _tmp1;
	const char* _tmp0;
	char* _tmp5;
	self = MUXER (base);
	g_return_if_fail (purpose != NULL);
	(*url) = NULL;
	fn = NULL;
	_tmp3 = NULL;
	_tmp2 = NULL;
	_tmp0 = NULL;
	if ((_tmp1 = c_alloc_channel (purpose, &_tmp0), fn = (_tmp2 = (_tmp3 = _tmp0, (_tmp3 == NULL ? NULL : g_strdup (_tmp3))), (fn = (g_free (fn), NULL)), _tmp2), _tmp1) < 0) {
		char* _tmp4;
		_tmp4 = NULL;
		fn = (_tmp4 = g_strdup (""), (fn = (g_free (fn), NULL)), _tmp4);
	}
	/*throw new MuxerError.System("alloc channel failed");*/
	_tmp5 = NULL;
	(*url) = (_tmp5 = g_strdup_printf ("file://%s", fn), ((*url) = (g_free ((*url)), NULL)), _tmp5);
	g_message ("muxercontrol.vala:42: %s: %s %s", "GetDevice", purpose, (*url));
	fn = (g_free (fn), NULL);
}


void muxer_trigger (gboolean on) {
	g_signal_emit_by_name (G_OBJECT (POWERED (muxer_server)), "power", on);
}


guint _dynamic_RequestName0 (DBusGProxy* obj, const char* param1, guint param2, GError** error) {
	guint result;
	dbus_g_proxy_call (obj, "RequestName", error, G_TYPE_STRING, param1, G_TYPE_UINT, param2, G_TYPE_INVALID, G_TYPE_UINT, &result, G_TYPE_INVALID);
	return result;
}


gint muxer_run (void) {
	GError * inner_error;
	inner_error = NULL;
	{
		GMainLoop* loop;
		DBusGConnection* conn;
		DBusGProxy* bus;
		guint request_name_result;
		Muxer* _tmp0;
		loop = g_main_loop_new (NULL, FALSE);
		conn = dbus_g_bus_get (DBUS_BUS_SYSTEM, &inner_error);
		if (inner_error != NULL) {
			goto __catch0_g_error;
		}
		bus = dbus_g_proxy_new_for_name (conn, "org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus");
		request_name_result = _dynamic_RequestName0 (bus, "org.pyneo.mux", ((guint) (0)), &inner_error);
		if (inner_error != NULL) {
			goto __catch0_g_error;
		}
		if (request_name_result != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
			inner_error = g_error_new (MUXER_ERROR, MUXER_ERROR_Startup, "not alone on the bus");
			if (inner_error != NULL) {
				goto __catch0_g_error;
			}
		}
		g_message ("muxercontrol.vala:56: starting server");
		_tmp0 = NULL;
		muxer_server = (_tmp0 = muxer_new (), (muxer_server == NULL ? NULL : (muxer_server = (g_object_unref (muxer_server), NULL))), _tmp0);
		dbus_g_connection_register_g_object (conn, "/org/pyneo/Muxer", G_OBJECT (muxer_server));
		g_main_loop_run (loop);
		(loop == NULL ? NULL : (loop = (g_main_loop_unref (loop), NULL)));
		(conn == NULL ? NULL : (conn = (dbus_g_connection_unref (conn), NULL)));
		(bus == NULL ? NULL : (bus = (g_object_unref (bus), NULL)));
	}
	goto __finally0;
	__catch0_g_error:
	{
		GError * e;
		e = inner_error;
		inner_error = NULL;
		{
			g_error ("muxercontrol.vala:62: %s", e->message);
		}
	}
	__finally0:
	;
	return 0;
}


/* HashTable<string,string>*/
Muxer* muxer_new (void) {
	Muxer * self;
	self = g_object_newv (TYPE_MUXER, 0, NULL);
	return self;
}


static void muxer_class_init (MuxerClass * klass) {
	muxer_parent_class = g_type_class_peek_parent (klass);
	G_OBJECT_CLASS (klass)->dispose = muxer_dispose;
}


static void muxer_powered_interface_init (PoweredIface * iface) {
	muxer_powered_parent_iface = g_type_interface_peek_parent (iface);
	iface->SetPower = muxer_real_SetPower;
	iface->GetPower = muxer_real_GetPower;
	iface->GetDevice = muxer_real_GetDevice;
}


static void muxer_instance_init (Muxer * self) {
}


static void muxer_dispose (GObject * obj) {
	Muxer * self;
	self = MUXER (obj);
	G_OBJECT_CLASS (muxer_parent_class)->dispose (obj);
}


GType muxer_get_type (void) {
	static GType muxer_type_id = 0;
	if (G_UNLIKELY (muxer_type_id == 0)) {
		static const GTypeInfo g_define_type_info = { sizeof (MuxerClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) muxer_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (Muxer), 0, (GInstanceInitFunc) muxer_instance_init };
		static const GInterfaceInfo powered_info = { (GInterfaceInitFunc) muxer_powered_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		muxer_type_id = g_type_register_static (G_TYPE_OBJECT, "Muxer", &g_define_type_info, 0);
		g_type_add_interface_static (muxer_type_id, TYPE_POWERED, &powered_info);
	}
	return muxer_type_id;
}



static void g_cclosure_user_marshal_BOOLEAN__STRING_BOOLEAN_POINTER (GClosure * closure, GValue * return_value, guint n_param_values, const GValue * param_values, gpointer invocation_hint, gpointer marshal_data) {
	typedef gboolean (*GMarshalFunc_BOOLEAN__STRING_BOOLEAN_POINTER) (gpointer data1, const char* arg_1, gboolean arg_2, gpointer arg_3, gpointer data2);
	register GMarshalFunc_BOOLEAN__STRING_BOOLEAN_POINTER callback;
	register GCClosure * cc;
	register gpointer data1, data2;
	gboolean v_return;
	cc = ((GCClosure *) (closure));
	g_return_if_fail (return_value != NULL);
	g_return_if_fail (n_param_values == 4);
	if (G_CCLOSURE_SWAP_DATA (closure)) {
		data1 = closure->data;
		data2 = param_values->data[0].v_pointer;
	} else {
		data1 = param_values->data[0].v_pointer;
		data2 = closure->data;
	}
	callback = ((GMarshalFunc_BOOLEAN__STRING_BOOLEAN_POINTER) ((marshal_data ? marshal_data : cc->callback)));
	v_return = callback (data1, g_value_get_string (param_values + 1), g_value_get_boolean (param_values + 2), g_value_get_pointer (param_values + 3), data2);
	g_value_set_boolean (return_value, v_return);
}


static void g_cclosure_user_marshal_BOOLEAN__STRING_POINTER_POINTER (GClosure * closure, GValue * return_value, guint n_param_values, const GValue * param_values, gpointer invocation_hint, gpointer marshal_data) {
	typedef gboolean (*GMarshalFunc_BOOLEAN__STRING_POINTER_POINTER) (gpointer data1, const char* arg_1, gpointer arg_2, gpointer arg_3, gpointer data2);
	register GMarshalFunc_BOOLEAN__STRING_POINTER_POINTER callback;
	register GCClosure * cc;
	register gpointer data1, data2;
	gboolean v_return;
	cc = ((GCClosure *) (closure));
	g_return_if_fail (return_value != NULL);
	g_return_if_fail (n_param_values == 4);
	if (G_CCLOSURE_SWAP_DATA (closure)) {
		data1 = closure->data;
		data2 = param_values->data[0].v_pointer;
	} else {
		data1 = param_values->data[0].v_pointer;
		data2 = closure->data;
	}
	callback = ((GMarshalFunc_BOOLEAN__STRING_POINTER_POINTER) ((marshal_data ? marshal_data : cc->callback)));
	v_return = callback (data1, g_value_get_string (param_values + 1), g_value_get_pointer (param_values + 2), g_value_get_pointer (param_values + 3), data2);
	g_value_set_boolean (return_value, v_return);
}



