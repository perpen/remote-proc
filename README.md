Total hack for running ps/vmstat/top locally, against the /proc of a remote machine.

The remote /proc fs is served over 9p (using https://github.com/chaos/diod), and
mounted locally using the builtin v9fs.

Uses an LD_PRELOAD hack to intercept calls to `open("/proc/...")`, could use unshare?

```
./dev srv
./dev mnt
./dev run ps
```
