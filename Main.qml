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
            text: "Start Calibration"
            onClicked: {state.text = "State: Calibration Started";
                dataHandler.startCalibration()
                console.log("Calibration started")
                xText: "X: " + accelerometer.x.toFixed(2)
                xValue: 0.5 + (accelerometer.x / 100)
                yText: "Y: " + accelerometer.y.toFixed(2)
                yValue: 0.5 + (accelerometer.y / 100)
                zText: "Z: " + gyroscope.z.toFixed(2)
                zValue: 0.5 + (gyroscope.z / 1000)
            }
        }

        CustomButton {
            text: "Start Pattern"
            onClicked: {state.text = "State: Pattern Started";
                accelerometer.accsampleCount = 0
                gyroscope.gyrosampleCount = 0
                dataHandler.startPattern();
                console.log("Start pattern")
                newmove.text = "Waiting for move";
            }
        }
        CustomButton {
            text: "Stop Pattern"
            onClicked: {state.text = "State: Pattern Stopped";
                dataHandler.stopPattern();
                console.log("Stop pattern")
            }
        }
        CustomButton {
            text: "Start Authentication"
            onClicked: {state.text = "State: Authentication Started";
                dataHandler.startAuthentication();
                console.log("Start Authentication")
                newmove.text = "Waiting for move";
            }
        }
        CustomButton {
            text: "Stop Authentication"
            onClicked: {state.text = "State: Authentication Stopped";
                dataHandler.stopAuthentication();
                console.log("Stop Authentication")
            }
        }

        Text {
            id: accelerometer_data
            text: qsTr("accelerometer data:")
        }

        ProgressXYBar {
            id: accbar
            Layout.fillWidth: true
            property bool caldone: false
            // property double xfiltered: 0
            // property double yfiltered: 0
            // property double zfiltered: 0
            // fontSize: root.fontSize
            xText: "X: " + accelerometer.x.toFixed(2)
            xValue: 0.5 + (accelerometer.x / 100)
            yText: "Y: " + accelerometer.y.toFixed(2)
            yValue: 0.5 + (accelerometer.y / 100)
        }

        Text {
            id: gyroscope_data
            text: qsTr("gyroscope data:")
        }

        ProgressZBar {
            id: gyrobar
            Layout.fillWidth: true
            // fontSize: root.fontSize
            zText: "Z: " + gyroscope.z.toFixed(2)
            zValue: 0.5 + (gyroscope.z / 1000)
        }

        Text {
            id: state
            text: qsTr("State: Idle")
        }

        Text {
            id: event
            text: qsTr("Event: ")
        }

        Text {
            id: calibration
            text: qsTr("Calibration: ")
        }
        
        Text {
            id: newmove
            text: qsTr("Last Move: ")
        }
        
        Text {
            id: authenticationout
            text: qsTr("Authentication Result: ")
        }
    }
    Accelerometer {
        id: accelerometer
        property real x: 0
        property real y: 0
        property real z: 0
        property real accsampleCount: 0

        active: true
        dataRate: 50

        onReadingChanged: {
            if(active == true){
                accsampleCount += 1
                x = (reading as AccelerometerReading).x
                y = (reading as AccelerometerReading).y
                z = (reading as AccelerometerReading).z
                console.log("X: " + x + " Y: " + y + " " + state.text  + " Count:" + accsampleCount
                            + " AccActive:" + active + " GyroActive:" + gyroscope.active)
                dataHandler.accReading(x,y)
            }
        }

    }

    Gyroscope {
        id: gyroscope
        property real x: 0
        property real y: 0
        property real z: 0
        property real gyrosampleCount: 0

        active: true
        dataRate: 50

        onReadingChanged: {
            if(active == true){
                gyrosampleCount += 1
                x = (reading as GyroscopeReading).x
                y = (reading as GyroscopeReading).y
                z = (reading as GyroscopeReading).z
                console.log("Z: " + z + " " + state.text + " Count:" + gyrosampleCount
                            + " AccActive:" + accelerometer.active + " GyroActive:" + active)
                dataHandler.gyroReading(z)
            }
        }
    }

    DataReadingHandler{
        id: dataHandler

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
            event.text = "Event: Calibration Done"
            calibration.text = "Calibration: " + dataHandler.calibration
            ProgressXYZBar.caldone = true
        }
        onNewpatternChanged: {
            event.text = "Event: New move recorded"
            newmove.text = "Last Move: " + dataHandler.newpattern
        }

        onFilteredXChanged: {
            accbar.xText = "X: " + dataHandler.filteredX.toFixed(2)
            accbar.xValue = 0.5 + (dataHandler.filteredX / 100)
        }
        onFilteredYChanged:{
            accbar.yText = "Y: " + dataHandler.filteredY.toFixed(2)
            accbar.yValue = 0.5 + (dataHandler.filteredY / 100)
        }
        onFilteredZChanged: {
            gyrobar.zText = "Z: " + dataHandler.filteredZ.toFixed(2)
            accbar.zValue = 0.5 + (dataHandler.filteredZ / 100)
        }
        onAuthresultChanged: {
            authenticationout.text = "Authentication Result: " + dataHandler.authresult
        }

        onMidMoveCalChanged: {
            if(dataHandler.midMoveCal)
            {
                event.text = "Event: Please wait for new Calibration";
            }
            else
            {
                event.text = "Event: Ready for next move";
            }
        }
    }
}
