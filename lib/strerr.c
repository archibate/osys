static
const char *err_msgs[] = {
	"success",
	"no such file or directory",
	"invalid argument / syscall",
	"not impelemented (yet)",
	"bad file descriptor",
	"not writable (read-only)",
	"not readable (write-only)",
	"wrong type of inode / spec",
	"bad block / chunk / file",
	"already existed",
	"out of open / slots / memory",
	"access violentation / segment fault",
};

const char *strerror(int err)
{
	if (err < 0)
		err = -err;
	if (err >= sizeof(err_msgs))
		return "error";
	return err_msgs[err];
}
