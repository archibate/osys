static
const char *err_msgs[] = {
	"success",
	"no such file or directory",
	"invalid argument / syscall",
	"not impelemented (yet)",
	"not writable (read-only)",
	"not readable (write-only)",
	"wrong type of inode / spec",
	"bad block / chunk",
	"out of open / slots / memory",
	"access violentation",
};

const char *strerror(int err)
{
	if (err < 0)
		err = -err;
	if (err >= sizeof(err_msgs))
		return "error";
	return err_msgs[err];
}
