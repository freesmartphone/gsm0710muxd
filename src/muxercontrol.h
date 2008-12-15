/*
valac --vapidir=src --pkg=gsm0710muxd --pkg=dbus-glib-1 --ccode src/muxercontrol.vala

DB="dbus-send --system --print-reply --type=method_call --dest=org.pyneo.mux /org/pyneo/Muxer"
$DB org.pyneo.Powered.GetPower string:xxx
$DB org.pyneo.Powered.SetPower string:xxx boolean:true
$DB org.pyneo.Powered.GetDevice string:xxx
*/

#ifndef __MUXERCONTROL_H__
#define __MUXERCONTROL_H__

#include <glib.h>
#include <glib-object.h>
#include <stdlib.h>
#include <string.h>

G_BEGIN_DECLS


#define TYPE_POWERED (powered_get_type ())
#define POWERED(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_POWERED, Powered))
#define IS_POWERED(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_POWERED))
#define POWERED_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), TYPE_POWERED, PoweredIface))

typedef struct _Powered Powered;
typedef struct _PoweredIface PoweredIface;

#define TYPE_MUXER (muxer_get_type ())
#define MUXER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_MUXER, Muxer))
#define MUXER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_MUXER, MuxerClass))
#define IS_MUXER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_MUXER))
#define IS_MUXER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_MUXER))
#define MUXER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_MUXER, MuxerClass))

typedef struct _Muxer Muxer;
typedef struct _MuxerClass MuxerClass;
typedef struct _MuxerPrivate MuxerPrivate;

typedef enum  {
	MUXER_ERROR_Startup,
	MUXER_ERROR_System,
	MUXER_ERROR_State
} MuxerError;
#define MUXER_ERROR muxer_error_quark ()
struct _PoweredIface {
	GTypeInterface parent_iface;
	void (*SetPower) (Powered* self, const char* purpose, gboolean on, GError** error);
	void (*GetPower) (Powered* self, const char* purpose, gboolean* on, GError** error);
	void (*GetDevice) (Powered* self, const char* purpose, char** url, GError** error);
};

/* HashTable<string,string>*/
struct _Muxer {
	GObject parent_instance;
	MuxerPrivate * priv;
};

struct _MuxerClass {
	GObjectClass parent_class;
};


GQuark muxer_error_quark (void);
void powered_SetPower (Powered* self, const char* purpose, gboolean on, GError** error);
void powered_GetPower (Powered* self, const char* purpose, gboolean* on, GError** error);
void powered_GetDevice (Powered* self, const char* purpose, char** url, GError** error);
GType powered_get_type (void);
void muxer_trigger (gboolean on);
gint muxer_run (void);
Muxer* muxer_new (void);
GType muxer_get_type (void);


G_END_DECLS

#endif
