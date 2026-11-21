import QtQuick
import QtQuick.Window

Window {
    width: 900
    height: 600

    visible: true
    title: "HeatControl"

    color: "#20242b"

    Text {
        anchors.centerIn: parent

        text: "HeatControl\nAplikacja uruchomiona"

        color: "white"
        font.pixelSize: 28

        horizontalAlignment: Text.AlignHCenter
    }
}
