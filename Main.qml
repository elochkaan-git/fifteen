import QtQuick
import QtQuick.Controls

Window {
    id: main_window
    width: 640
    height: 480
    visible: true
    title: qsTr("Fifteen")

    property int cell_size: 50

    StackView {
        id: stack
        initialItem: main_menu
        anchors.fill: parent
    }

    Component {
        id: main_menu
        MainMenu {
            onPlayClicked: stack.push(game)
            onScoreboardClicked: stack.push(scoreboard)
            onExitClicked: Qt.quit()
        }
    }

    Component {
        id: game
        Game {
            onQuitGame: stack.pop()
        }
    }

    Component {
        id: scoreboard
        Score {
            onBack: stack.pop()
        }
    }
}
