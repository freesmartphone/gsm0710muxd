/*
valac --vapidir=src --pkg=gsm0710muxd --pkg=dbus-glib-1 --ccode src/muxercontrol.vala

DB="dbus-send --system --print-reply --type=method_call --dest=org.pyneo.mux /org/pyneo/Muxer"
$DB org.pyneo.Powered.GetPower string:xxx
$DB org.pyneo.Powered.SetPower string:xxx boolean:true
$DB org.pyneo.Powered.GetDevice string:xxx
*/
public errordomain MuxerError {
	Startup,
	System,
	State
}

[DBus(name = "org.pyneo.Powered")]
public interface Powered : GLib.Object {
	/* switch power on/off. */
	public abstract void SetPower(string purpose, bool on) throws GLib.Error;
	/* return if power is already on/off. */
	public abstract void GetPower(string purpose, out bool on) throws GLib.Error;
	/* */
	public signal void power(bool on); // TODO upercase cores! rename it to Power() if that is fixed
	/* */
	public abstract void GetDevice(string purpose, out string url) throws GLib.Error;
	// HashTable<string,string>
}

public class Muxer : GLib.Object, Powered {
	public void SetPower(string purpose, bool on) throws GLib.Error {
		GLib.message("%s: %s %d", "SetPower", purpose, on);
		gsm0710muxd.c_set_power(purpose, on);
	}
	public void GetPower(string purpose, out bool on) throws GLib.Error {
		on = gsm0710muxd.c_get_power(purpose);
		GLib.message("%s: %s %d", "GetPower", purpose, on);
	}
	public void GetDevice(string purpose, out string url) throws GLib.Error {
		string fn;
		if (gsm0710muxd.c_alloc_channel(purpose, out fn) < 0)
			fn = ""; //throw new MuxerError.System("alloc channel failed");
		url = "file://%s".printf(fn);
		GLib.message("%s: %s %s", "GetDevice", purpose, url);
	}
	static Muxer server;
	public static void trigger(bool on) {
		server.power(on);
		}
	public static int run() {
		try {
			GLib.MainLoop loop = new GLib.MainLoop(null, false);
			var conn = DBus.Bus.get(DBus.BusType.SYSTEM);
			dynamic DBus.Object bus = conn.get_object("org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus");
			uint request_name_result = bus.RequestName("org.pyneo.mux", (uint)0);
			if (request_name_result != DBus.RequestNameReply.PRIMARY_OWNER)
				throw new MuxerError.Startup("not alone on the bus");
			GLib.message("starting server");
			server = new Muxer();
			conn.register_object("/org/pyneo/Muxer", server);
			loop.run();
		}
		catch (GLib.Error e) {
			GLib.error("%s", e.message);
        }
		return 0;
	}
	//static int main(string[] args) { return run(); }
}
