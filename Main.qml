import QtQuick
import com.textbox 1.0


Window {
    id: root;
    width: 420
    height: 760
    visible: true
    title: qsTr("Hello World")
    color: "pink";

    Rectangle {
        id: rec1;
        width: 200;
        height: 200;
        color: "magenta";

        anchors {
            top: parent.verticalCenter;
            horizontalCenter: parent.horizontalCenter;
            topMargin: 20;
        }

        radius: 10;

        MouseArea {
            id: mymouse;
            anchors.fill: parent;
            hoverEnabled: true;
            onClicked: myfunc();
        }
    }

    Rectangle {
        id: therectangle;
        antialiasing: true;
        width: 200;
        height: 200;
        color: "magenta";
        // anchors.centerIn: parent;
        radius: 10;
        anchors {
            bottom: parent.verticalCenter;
            horizontalCenter: parent.horizontalCenter;
            bottomMargin: 20;
        }

        Text {
            id: thetext
            text: "text"
            anchors.centerIn: parent;
        }

        MouseArea {
            id: hismouse;
            anchors.fill: parent;
            hoverEnabled: true;
            onClicked: textboxhandler.clicked(thetext.font.pixelSize);
        }
    }

    Rectangle {
        id: hidrect
        property bool isvisible: false
        visible: isvisible
        color: "red"
        width: parent.width;
        height: 20
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            margins: 20
        }

    }

    TextBoxHandler {
        id: textboxhandler
        onPixelszChanged: {
            thetext.font.pixelSize = pixelsz;
            console.log("hellooooooooooooooo");
        }
    }

    function myfunc() {
        thetext.font.pixelSize++;
        console.log("I am clicked!");
        hidrect.isvisible = !hidrect.isvisible
    }
}
