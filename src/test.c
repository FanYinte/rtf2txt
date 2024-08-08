#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "textproc.h"
// �����Ѿ������� internal_source �� rtf_part �ṹ��
// �Լ� get_next_part ����
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
    static char text[1024]; // ��̬���������ɸ�����Ҫ������С
    text[0] = '\0'; // ��ʼ��������

    rtf_part *part;
    while ((part = get_next_part(src)) != NULL) {
    	//printf("1\n");
        //����ȡ����,�õ�����GBK���룬��Ҫ����
        if (part->type == PART_TYPE_HEX_CHAR) {
        	//printf("1\n");
            //strcat(text, part->content.content); // ���� content ��һ�������ַ����Ľṹ��
            printf("%s",part->content.content);
        }
        //��ȡӢ�ģ������ֿ��Ʒ���
        /*if (part->type == PART_TYPE_TEXT) {
        	//printf("1\n");
            //strcat(text, part->content.content); // ���� content ��һ�������ַ����Ľṹ��
            printf("%s",part->content.content);
        }*/
    }
	//printf("1\n");
    return text;
}

int main() {
    // �����Ѿ���ʼ���� internal_source �ṹ�� src
    // internal_source src;
    // init_source(&src); // ������һ����ʼ������
    char file_path[256]; // �����ļ�·���Ϊ255���ַ�
    // ��ʾ�û������ļ�·��
    printf("please enter file_path: ");
    // ��ȡ�û�������ļ�·��
    // ȥ�������ַ���ĩβ�Ļ��з�
    fgets(file_path, sizeof(file_path), stdin);
    size_t len = strlen(file_path);
    if (len > 0 && file_path[len - 1] == '\n') {
        file_path[len - 1] = '\0';
    }
    FILE *file = fopen(file_path, "rb");

    //��ʼ��src
    source src;
    src.object = (void*)file;
    src.read = read_source;
    src.open = open_source;
    src.close = close_source;

    //��ʼ��log
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

    // ������Դ
    // cleanup_source(&src); // ������һ��������

    return 0;
}

