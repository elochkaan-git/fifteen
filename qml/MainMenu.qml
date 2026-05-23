import QtQuick
import QtQuick.Controls

Item {
    signal playClicked()
    signal exitClicked()
    signal scoreboardClicked()

    Column {
        spacing: 8
        anchors.centerIn: parent
        width: 320

        TextArea {
            text: "Выберите режим игры"
        }

        ComboBox {
            id: board_selector
            width: parent.width
            currentIndex: 1
            model: ListModel {
                ListElement { text: "Классический (3x3)"; value: 0 }
                ListElement { text: "Классический (4x4)"; value: 1 }
                ListElement { text: "На время (кастомный размер и время)"; value: 2 }
            }
            textRole: "text"
        }

        Row {
            spacing: 8
            visible: board_selector.currentIndex === 2

            Text { text: "Строк:"; }
            TextField {
                id: rowsInput
                width: 40
                text: "2"
                validator: IntValidator {
                    bottom: 2
                    top: Math.floor((0.75 * main_window.height) / main_window.cell_size)
                }
            }

            Text { text: "Столбцов:"; }
            TextField {
                id: colsInput
                width: 40
                text: "2"
                validator: IntValidator {
                    bottom: 2
                    top: Math.floor((0.75 * main_window.width) / main_window.cell_size)
                }
            }

            Text { text: "Время (в секундах):"; }
            TextField {
                id: timeInput
                width: 40
                text: "10"
                validator: IntValidator {
                    bottom: 1
                    top: 5999
                }
            }
        }

        Button {
            id: play_btn
            text: "Играть"
            enabled: rowsInput.acceptableInput && colsInput.acceptableInput
            onClicked: () => {
                if (board_selector.currentIndex === 2) {
                    board.change_game(board_selector.currentIndex, rowsInput.text, colsInput.text, timeInput.text)
                } else {
                    board.change_game(board_selector.currentIndex)
                }
                playClicked()
            }
        }

        Button {
            text: "Таблица лидеров"
            onClicked: scoreboardClicked()
        }

        Button {
            text: "Выход"
            onClicked: exitClicked()
        }
    }
}
