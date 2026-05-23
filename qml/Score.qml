import QtQuick
import QtQuick.Controls

Item {
    signal back()

    Rectangle {
        anchors.centerIn: parent
        width: Math.min(parent.width * 0.85, 600)
        height: parent.height * 0.82
        radius: 16
        color: "#e6f1fb"
        border.color: "#b5d4f4"
        border.width: 1

        Column {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 8

            Text {
                text: "Таблица лидеров"
                font.pointSize: 16
                font.bold: true
                color: "#0c447c"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            HorizontalHeaderView {
                id: header
                width: parent.width
                syncView: tableView
                delegate: Rectangle {
                    implicitHeight: 36
                    color: "#b5d4f4"
                    radius: 6
                    Text {
                        anchors.centerIn: parent
                        text: model.display
                        color: "#042c53"
                        font.bold: true
                        font.pointSize: 11
                    }
                }
            }

            TableView {
                id: tableView
                width: parent.width
                height: parent.height - 36 - 16 - 36 - 16
                clip: true
                model: scoreboardModel
                rowHeightProvider: function(row) { return 34 }
                columnWidthProvider: function(column) { return width / 5 }
                delegate: Rectangle {
                    color: row % 2 === 0 ? "#f0f7fd" : "#ddeaf8"
                    Text {
                        anchors.centerIn: parent
                        text: model.display ?? ""
                        color: "#185fa5"
                        font.pointSize: 10
                    }
                }
            }
        }
    }

    Button {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 24
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Назад"
        onClicked: back()
    }
}
