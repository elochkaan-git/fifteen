import QtQuick
import QtQuick.Controls

Item {
    signal quitGame()
    focus: true

    Keys.onPressed: (event) => {
        if (event.key === Qt.Key_Up)
            board.move_cell(2)
        else if (event.key === Qt.Key_Right)
            board.move_cell(3)
        else if (event.key === Qt.Key_Down)
            board.move_cell(0)
        else if (event.key === Qt.Key_Left)
            board.move_cell(1)
        else if (event.text === "q")
            quitGame()
        else if (event.text === "Z")
            board.shuffle()
    }
    Column {
        spacing: 10
        anchors.centerIn: parent

        Grid {
            id: grid
            columns: board.get_cols()
            Repeater {
                model: board
                Rectangle {
                    width: 50
                    height: 50
                    color: model.isNull ? "transparent" : "steelblue"
                    border.color: model.isNull ? "transparent" : "white"
                    border.width: 2
                    Text {
                        text: model.isNull ? "" : model.value
                        anchors.centerIn: parent
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: board.move_cell(index % board.get_cols(), index / board.get_cols())
                    }
                }
            }
        }
        Timer {
            interval: 1000
            running: !board.is_win()
            repeat: true
            onTriggered: board.tick()
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Шаги: %1   Время: %2".arg(board.steps).arg(board.time)
            color: "#cdd6f4"
        }
        Button {
            id: button_shuffle
            text: "Shuffle"
            onClicked: board.shuffle()
        }
        Connections {
            target: board
            function onBoardChanged() {
                winOverlay.visible = board.is_win()
            }
        }
        Text {
            id: tip
            text: "q - для выхода из игры\nZ - для перемешивания костяшек"
        }
    }
    Rectangle {
        id: winOverlay
        anchors.fill: parent
        color: "#aa000000"
        visible: false

        Column {
            anchors.centerIn: parent
            spacing: 16

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Победа!"
                font.pointSize: 24
                font.bold: true
                color: "lightgreen"
            }

            TextField {
                id: nameField
                anchors.horizontalCenter: parent.horizontalCenter
                width: 200
                placeholderText: "Введите имя..."
                horizontalAlignment: Text.AlignHCenter
                onVisibleChanged: if (visible) { clear(); forceActiveFocus() }
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Сохранить"
                enabled: nameField.text.trim().length > 0
                onClicked: {
                    board.save_result(nameField.text.trim())
                    quitGame()
                }
            }
        }
    }
}
