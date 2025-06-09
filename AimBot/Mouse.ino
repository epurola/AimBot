#include <Mouse.h>

void setup() {
    Serial.begin(9600);
    while (!Serial);  // Wait for Serial Monitor to open
    Mouse.begin();
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        int commaIndex = input.indexOf(',');
        if (commaIndex > 0) {
            String dxStr = input.substring(0, commaIndex);
            String dyStr = input.substring(commaIndex + 1);

            int dx = dxStr.toInt();
            int dy = dyStr.toInt();

            MoveX(dx);
            MoveY(dy);
        }
    }
}

void MoveX(int dx)
{
    int moves;
    int remainder;
    const int maxMove = 127;

    if (dx == 0) return;

    int sign = (dx > 0) ? 1 : -1;
    int absDx = abs(dx);

    if (absDx <= maxMove) {
        Mouse.move(dx, 0, 0);
        return;
    }

    moves = absDx / maxMove;
    remainder = absDx % maxMove;

    for (int i = 0; i < moves; i++) {
        Mouse.move(sign * maxMove, 0, 0);
    }
    if (remainder > 0) {
        Mouse.move(sign * remainder, 0, 0);
    }
}

void MoveY(int dy)
{
    int moves;
    int remainder;
    const int maxMove = 127;

    if (dy == 0) return;

    int sign = (dy > 0) ? 1 : -1;
    int absDy = abs(dy);

    if (absDy <= maxMove) {
        Mouse.move(0, dy, 0);
        return;
    }

    moves = absDy / maxMove;
    remainder = absDy % maxMove;

    for (int i = 0; i < moves; i++) {
        Mouse.move(0, sign * maxMove, 0);
    }
    if (remainder > 0) {
        Mouse.move(0, sign * remainder, 0);
    }
}