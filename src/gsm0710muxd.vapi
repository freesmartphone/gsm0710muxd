namespace gsm0710muxd {
	[CCode (cname = "c_get_power")]
	public bool c_get_power (string origin);
	[CCode (cname = "c_set_power")]
	public void c_set_power (string origin, bool on);
	[CCode (cname = "c_alloc_channel")]
	public int c_alloc_channel (string origin, out weak string fn);
}
