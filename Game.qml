import QtQuick
import QtQuick.Controls

Item {
    signal quitGame()
    focus: true

    Keys.onPressed: (event) => {
        if (winText.visible) {
            quitGame()
            event.accepted = true
            return
        } else if (event.key === Qt.Key_Up)
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

        Button {
            id: button_shuffle
            text: "Shuffle"
            onClicked: board.shuffle()
        }

        Connections {
            target: board
            function onBoardChanged() {
                if (board.is_win())
                    winText.visible = true
                else
                    winText.visible = false
            }
        }

        Text {
            id: tip
            text: "q - для выхода из игры\nZ - для перемешивания костяшек"
        }

        Text {
            id: winText
            text: "Победа!"
            font.pointSize: 18
            font.bold: true
            color: "green"
            visible: false
        }
    }
}
