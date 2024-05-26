pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtSensors
import com.DataReadingHandler.h 1.0
import com.logger.h


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
            text: "Start Calibration"
            onClicked: {outfield.text = "Calibration Started";
                dataHandler.startCalibration()}
        }

        CustomButton {
            text: "Start Pattern"
            onClicked: {outfield.text = "Pattern Started";
                dataHandler.startPattern();
            }
        }
        CustomButton {
            text: "Stop Pattern"
            onClicked: {outfield.text = "Pattern Stopped";
                dataHandler.stopPattern();
            }
        }
        CustomButton {
            text: "Start Authentication"
        }
        CustomButton {
            text: "Stop Authentication"
        }

        Text {
            id: accelerometer_data
            text: qsTr("accelerometer data:")
        }

        ProgressXYZBar {
            Layout.fillWidth: true
            // fontSize: root.fontSize
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
            // fontSize: root.fontSize
            xText: "X: " + gyroscope.x.toFixed(2)
            xValue: 0.5 + (gyroscope.x / 1000)
            yText: "Y: " + gyroscope.y.toFixed(2)
            yValue: 0.5 + (gyroscope.y / 1000)
            zText: "Z: " + gyroscope.z.toFixed(2)
            zValue: 0.5 + (gyroscope.z / 1000)
        }

        Text {
            id: outfield
            text: qsTr("Welcome")
        }

        Text {
            id: movementout
            text: qsTr("Movement: 0")
        }
        
        Text {
            id: yvelocityout
            text: qsTr("Velocity Y: 0")
        }
        
        Text {
            id: xvelocityout
            text: qsTr("Velocity X: 0")
        }

        Text {
            id: rotationout
            text: qsTr("Rotation: 0")
        }

        Text {
            id: authenticationout
            text: qsTr("Authentication: 0")
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
            dataHandler.accReading(x,y)
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
            dataHandler.gyroReading(z)
        }
    }

    DataReadingHandler{
        id: dataHandler
        onMovementChanged: {
            console.log("Movement: " + dataHandler.movement)
            movementout.text = "Movement: " + dataHandler.movement
        }

        onRotationZChanged: {
            console.log("Rotation: " + dataHandler.rotationZ)
            rotationout.text = "Rotation: " + dataHandler.rotationZ
        }

        onVelocityXChanged: {
            console.log("VelocityX: " + dataHandler.velocityX)
            xvelocityout.text = "Velocity X: " + dataHandler.velocityX
        }

        onVelocityYChanged: {
            console.log("VelocityY: " + dataHandler.velocityY)
            yvelocityout.text = "Velocity Y: " + dataHandler.velocityY
        }

        onAccActiveChanged: {
            if (dataHandler.accActive) {
                accelerometer.start();
            } else {
                accelerometer.stop();
            }
        }
        onGyroActiveChanged: {
            if (dataHandler.gyroActive) {
                gyroscope.start();
            } else {
                gyroscope.stop();
            }
        }
        onCalibrationChanged: {
            console.log("Calibration: " + dataHandler.calibration)
            authenticationout.text = "Calibration: " + dataHandler.calibration
        }
    }
}
