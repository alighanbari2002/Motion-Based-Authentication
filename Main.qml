pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtSensors
import com.DataReadingHandler.h 1.0

ApplicationWindow {
    id: root;
    width: 420
    height: 760
    visible: true
    title: qsTr("SensorBasedAuth")
    color: "white";

    readonly property int defaultFontSize: 22
    // readonly property int imageSize: width / 2

    header : ToolBar {
        id: header
        background: Rectangle {
                color: "lightblue"
            }
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            Label {
                id: heading
                text: root.title
                font.pixelSize: root.defaultFontSize
                font.weight: Font.Medium
                verticalAlignment: Qt.AlignVCenter
                Layout.alignment: Qt.AlignCenter
                Layout.preferredHeight: 55
                color: "black"
            }
        }
    }

    ColumnLayout{
        anchors.fill: parent
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        spacing: 10

        component CustomButton: Button {
            // highlighted: true
            font.pixelSize: root.defaultFontSize
            font.letterSpacing: 1.5

            Layout.alignment: Qt.AlignCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        CustomButton {
            text: "Start Pattern"
            // onClicked: stack.pusher("Accelerometer.qml")
            // enabled: SensorSupport.hasAccelerometer()
        }
        CustomButton {
            text: "Stop Pattern"
            // onClicked: stack.pusher("Proximity.qml")
            // enabled: SensorSupport.hasProximity()
        }
        CustomButton {
            text: "Start Authentication"
            // onClicked: stack.pusher("Compass.qml")
            // enabled: SensorSupport.hasCompass()
        }
        CustomButton {
            text: "Stop Authentication"
            // onClicked: stack.pusher("Magnetometer.qml")
            // enabled: SensorSupport.hasMagnetometer()
        }

        Text {
            id: accelerometer_data
            text: qsTr("accelerometer data:")
        }

        ProgressXYZBar {
            Layout.fillWidth: true
            fontSize: root.fontSize
            xText: "X: " + accelerometer.x.toFixed(2)
            xValue: 0.5 + (accelerometer.x / 100)
            yText: "Y: " + accelerometer.y.toFixed(2)
            yValue: 0.5 + (accelerometer.y / 100)
            zText: "Z: " + accelerometer.z.toFixed(2)
            zValue: 0.5 + (accelerometer.z / 100)
        }

        Text {
            id: gyroscope_data
            text: qsTr("gyroscope data:")
        }

        ProgressXYZBar {
            Layout.fillWidth: true
            fontSize: root.fontSize
            xText: "X: " + gyroscope.x.toFixed(2)
            xValue: 0.5 + (gyroscope.x / 1000)
            yText: "Y: " + gyroscope.y.toFixed(2)
            yValue: 0.5 + (gyroscope.y / 1000)
            zText: "Z: " + gyroscope.z.toFixed(2)
            zValue: 0.5 + (gyroscope.z / 1000)
        }
    }
    Accelerometer {
        id: accelerometer
        property real x: 0
        property real y: 0
        property real z: 0

        active: true
        dataRate: 25

        onReadingChanged: {
            x = (reading as AccelerometerReading).x
            y = (reading as AccelerometerReading).y
            z = (reading as AccelerometerReading).z
        }

    }

    Gyroscope {
        id: gyroscope
        property real x: 0
        property real y: 0
        property real z: 0

        active: true
        dataRate: 25

        onReadingChanged: {
            x = (reading as GyroscopeReading).x
            y = (reading as GyroscopeReading).y
            z = (reading as GyroscopeReading).z
        }
    }

    function myfunc() {
        thetext.font.pixelSize++;
        console.log("I am clicked!");
        hidrect.isvisible = !hidrect.isvisible
    }
}



