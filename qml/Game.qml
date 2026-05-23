import QtQuick
import QtQuick.Controls

Item {
    signal quitGame()
    focus: true

    Keys.onPressed: (event) => {
        if      (event.key === Qt.Key_Up)    board.move_cell(2)
        else if (event.key === Qt.Key_Right) board.move_cell(3)
        else if (event.key === Qt.Key_Down)  board.move_cell(0)
        else if (event.key === Qt.Key_Left)  board.move_cell(1)
        else if (event.text === "q")         quitGame()
        else if (event.text === "Z")         board.shuffle()
    }

    Timer {
        interval: 100
        running: true
        repeat: true
        onTriggered: {
            board.tick()
            if (board.status === 1) overlay.show("Победа!",  "lightgreen")
            if (board.status === 2) overlay.show("Поражение!", "tomato")
        }
    }

    Column {
        spacing: 10
        anchors.centerIn: parent

        Grid {
            columns: board.get_cols()
            Repeater {
                model: board
                Rectangle {
                    width: 50; height: 50
                    color: model.isNull ? "transparent" : board.status === 1 ? "green" : board.status === 2 ? "crimson" : "steelblue"
                    border.color: "white"
                    border.width: 2
                    Text {
                        text: model.isNull ? "" : model.value
                        anchors.centerIn: parent
                        color: "white"
                    }
                }
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Шаги: %1   Время: %2".arg(board.steps).arg(board.time)
            color: "#cdd6f4"
        }

        Button {
            text: "Shuffle"
            onClicked: board.shuffle()
        }

        Text {
            text: "q — выход   Z — перемешать"
            color: "#6c7086"
        }
    }

    Rectangle {
        id: overlay
        anchors.fill: parent
        color: "#aa000000"
        visible: false

        property bool isWin: false

        function show(msg, clr) {
            if (visible) return
            isWin = board.status === 1
            overlayText.text = msg
            overlayText.color = clr
            visible = true
        }

        Column {
            anchors.centerIn: parent
            spacing: 16

            Text {
                id: overlayText
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 24
                font.bold: true
            }

            TextField {
                id: nameField
                anchors.horizontalCenter: parent.horizontalCenter
                visible: overlay.isWin
                width: 200
                placeholderText: "Введите имя..."
                horizontalAlignment: Text.AlignHCenter
                onVisibleChanged: if (visible) { clear(); forceActiveFocus() }
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Сохранить"
                visible: overlay.isWin
                enabled: nameField.text.trim().length > 0
                onClicked: {
                    board.save_result(nameField.text.trim())
                    quitGame()
                }
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Выйти"
                onClicked: quitGame()
            }
        }
    }
}
