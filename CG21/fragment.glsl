#version 330 core

//--- out_Color: ���ؽ� ���̴����� �Է¹޴� ���� ��
//--- FragColor: ����� ������ ������ ������ ���۷� ���� ��.

in vec3 color; //--- ���ؽ� ���̴����Լ� ���� ����
out vec4 outcolor; //--- ���� ���


void main(void)
{
outcolor = vec4(color, 1.0);
}