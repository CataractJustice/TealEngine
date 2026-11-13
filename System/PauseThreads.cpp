/*#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <sys/syscall.h>
#include <vector>
#include <string>
#include <iostream>

pid_t gettid() {
	return syscall(SYS_gettid);
}

std::vector<pid_t> get_other_threads() {
	std::vector<pid_t> tids;
	DIR* dir = opendir("/proc/self/task");
	if (!dir) return tids;

	pid_t self = gettid();
	struct dirent* ent;
	while ((ent = readdir(dir)) != nullptr) {
		if (ent->d_type != DT_DIR) continue;
		pid_t tid = atoi(ent->d_name);
		if (tid > 0 && tid != self)
			tids.push_back(tid);
	}
	closedir(dir);
	return tids;
}

void pause_other_threads() {
	for (pid_t tid : get_other_threads())
		syscall(SYS_tgkill, getpid(), tid, SIGSTOP);
}

void resume_other_threads() {
	for (pid_t tid : get_other_threads())
		syscall(SYS_tgkill, getpid(), tid, SIGCONT);
}*/