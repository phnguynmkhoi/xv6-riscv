#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // Mở thư mục hoặc tệp
    if ((fd = open(path, 0)) < 0) {
        printf("find: cannot open %s\n", path);
        return;
    }

    // Lấy thông tin tệp hoặc thư mục
    if (fstat(fd, &st) < 0) {
        printf("find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // Nếu là thư mục thì đọc các tệp và thư mục con
    if (st.type == T_DIR) {
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("find: path too long\n");
            close(fd);
            return;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';

        // Đọc từng mục trong thư mục
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0)
                continue;
            // Bỏ qua "." và ".."
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0) {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            // So sánh tên tệp với tên cần tìm
            if (strcmp(de.name, filename) == 0) {
                printf("%s\n", buf);
            }
            // Đệ quy tìm trong thư mục con
            if (st.type == T_DIR) {
                find(buf, filename);
            }
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: find <path> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
