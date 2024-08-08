#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "textproc.h"
// 假设已经定义了 internal_source 和 rtf_part 结构体
// 以及 get_next_part 函数
size_t read_source(source *src, char *buf, size_t size) {
    FILE *file = (FILE *)src->object;
    return fread(buf, 1, size, file);
}

void open_source(source *src) {
    FILE *file = (FILE *)src->object;
    fseek(file, 0, SEEK_SET);
}

void close_source(source *src) {
    FILE *file = (FILE *)src->object;
    fclose(file);
}

void debug(observer *obs, const char *msg) {
    printf("DEBUG: %s\n", msg);
}

void warning(observer *obs, int id, const char *msg) {
    printf("WARNING(%d): %s\n", id, msg);
}

void error(observer *obs, int id, const char *msg) {
    printf("ERROR(%d): %s\n", id, msg);
}


char* extract_text(internal_source *src) {
    static char text[1024]; // 静态缓冲区，可根据需要调整大小
    text[0] = '\0'; // 初始化缓冲区

    rtf_part *part;
    while ((part = get_next_part(src)) != NULL) {
    	//printf("1\n");
        //仅提取中文,得到的是GBK编码，需要解码
        if (part->type == PART_TYPE_HEX_CHAR) {
        	//printf("1\n");
            //strcat(text, part->content.content); // 假设 content 是一个包含字符串的结构体
            printf("%s",part->content.content);
        }
        //提取英文（含部分控制符）
        /*if (part->type == PART_TYPE_TEXT) {
        	//printf("1\n");
            //strcat(text, part->content.content); // 假设 content 是一个包含字符串的结构体
            printf("%s",part->content.content);
        }*/
    }
	//printf("1\n");
    return text;
}

int main() {
    // 假设已经初始化了 internal_source 结构体 src
    // internal_source src;
    // init_source(&src); // 假设有一个初始化函数
    char file_path[256]; // 假设文件路径最长为255个字符
    // 提示用户输入文件路径
    printf("please enter file_path: ");
    // 读取用户输入的文件路径
    // 去掉输入字符串末尾的换行符
    fgets(file_path, sizeof(file_path), stdin);
    size_t len = strlen(file_path);
    if (len > 0 && file_path[len - 1] == '\n') {
        file_path[len - 1] = '\0';
    }
    FILE *file = fopen(file_path, "rb");

    //初始化src
    source src;
    src.object = (void*)file;
    src.read = read_source;
    src.open = open_source;
    src.close = close_source;

    //初始化log
    observer log;
    log.object = NULL;
    log.debug = debug;
    log.warning = warning;
    log.error = error;

    internal_source isrc;
    isrc.iface = &src;
    isrc.log = &log;
    isrc.use_current_part = 0;
	isrc.last_returned_ch = 0;
	isrc.ungot_char = -1;
	isrc.ungot_char2 = -1;
	isrc.ungot_char3 = -1;
	isrc.buf = 0;
	isrc.index = 0;
	isrc.buf_size = 0;
	isrc.read_bytes = 0;
    char *text = extract_text(&isrc);
    printf("Extracted Text: %s\n", text);

    // 清理资源
    // cleanup_source(&src); // 假设有一个清理函数

    return 0;
}

