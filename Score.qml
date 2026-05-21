import QtQuick
import QtQuick.Controls

Item {
    signal back()

    Rectangle {
        id: table
        anchors.fill: parent
        color: "#1e1e2e"
        anchors.bottomMargin: main_window.height / 3

        HorizontalHeaderView {
            id: header
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            syncView: tableView
            delegate: Rectangle {

                implicitHeight: 32
                color: "#313244"
                Text {
                    anchors.centerIn: parent
                    text: model.display
                    color: "#cdd6f4"
                    font.bold: true
                }
            }
        }

        TableView {
            id: tableView
            anchors.top: header.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            model: scoreboardModel

            columnWidthProvider: function(column) {
                return width / 5
            }

            delegate: Rectangle {
                implicitHeight: 32
                color: row % 2 === 0 ? "#181825" : "#1e1e2e"
                Text {
                    anchors.centerIn: parent
                    text: model.display ?? ""
                    color: "#cdd6f4"
                }
            }
        }
    }

    Button {
        anchors.top: table.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Назад"
        onClicked: back()
    }
}
