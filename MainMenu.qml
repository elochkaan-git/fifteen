import QtQuick
import QtQuick.Controls

Item {
    signal playClicked()
    signal exitClicked()

    Column {
        spacing: 8
        anchors.centerIn: parent
        width: 320

        TextArea {
            text: "Выберите поле"
        }

        ComboBox {
            id: board_selector
            width: parent.width
            currentIndex: 2
            model: ListModel {
                ListElement { text: "Маленькое (3x3)"; value: 0 }
                ListElement { text: "Классическое (4x4)"; value: 1 }
                ListElement { text: "Большое (8x8)"; value: 2 }
                ListElement { text: "Прямоугольное (3x5)"; value: 3 }
                ListElement { text: "Свой размер..."; value: 4 }
            }
            textRole: "text"
        }

        Row {
            spacing: 8
            visible: board_selector.currentIndex === 4

            Text { text: "Строк:"; }
            TextField {
                id: rowsInput
                width: 40
                text: "4"
                validator: IntValidator {
                    bottom: 2
                    top: Math.floor((0.75 * main_window.height) / main_window.cell_size)
                }
            }

            Text { text: "Столбцов:"; }
            TextField {
                id: colsInput
                width: 40
                text: "4"
                validator: IntValidator {
                    bottom: 2
                    top: Math.floor((0.75 * main_window.width) / main_window.cell_size)
                }
            }
        }

        Button {
            id: play_btn
            text: "Играть"
            enabled: rowsInput.acceptableInput && colsInput.acceptableInput
            onClicked: () => {
                if (board_selector.currentIndex === 4) {
                    board.change_factory(board_selector.currentIndex, rowsInput.text, colsInput.text)
                } else {
                    board.change_factory(board_selector.currentIndex)
                }
                playClicked()
            }
        }

        Button {
            text: "Выход"
            onClicked: exitClicked()
        }
    }
}
