#pragma once
class ConsoleWindow
{
public:
    static void Create() {
        // �ܼ� ������ ����
        AllocConsole();

        // ǥ�� ����� ��Ʈ���� �ַܼ� �����̷�Ʈ
        FILE* pCout;
        freopen_s(&pCout, "CONOUT$", "w", stdout);

        FILE* pCin;
        freopen_s(&pCin, "CONIN$", "r", stdin);

        // ǥ�� ���� ��Ʈ���� �ַܼ� �����̷�Ʈ
        FILE* pCerr;
        freopen_s(&pCerr, "CONOUT$", "w", stderr);

        // C++ ��Ʈ���� ����ȭ
        std::ios::sync_with_stdio();
    }

    static void Destroy() {
        // ��Ʈ�� �ݱ�
        fclose(stdout);
        fclose(stdin);
        fclose(stderr);

        // �ܼ� ������ ����
        FreeConsole();
    }
};

