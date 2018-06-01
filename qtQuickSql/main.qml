import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    toolBar: ToolBar {

        style: ToolBarStyle{

            background: Rectangle {
              implicitWidth: 100
              implicitHeight: 60
              border.color: "#999"
              color:"#d3daed"
            }
        }

      RowLayout {

          anchors.left: parent.left
          anchors.top:parent.top
          anchors.margins: 5
          spacing: 20
          ToolButton{
              implicitHeight: 35
              implicitWidth: 35
              iconSource: "pic/load.png"
              Text{
                  text:"加载"
                  anchors.top:parent.bottom
                  anchors.horizontalCenter: parent.horizontalCenter
              }
              onClicked: {
                  //filechoose.show()
              }

          }

          ToolButton {
              implicitHeight: 35
              implicitWidth: 35
              iconSource: "pic/search.png"
              Text{
                  text:"检索"
                  anchors.top:parent.bottom
                  anchors.horizontalCenter: parent.horizontalCenter
              }
              onClicked: {
                 //search.show()
              }
          }
          ToolButton{
              implicitHeight: 35
              implicitWidth: 35
              iconSource: "pic/book.png"
              Text{
                  text:"书签"
                  anchors.top:parent.bottom
                  anchors.horizontalCenter: parent.horizontalCenter
              }
              onClicked: {
                  //bookm.show()
              }
          }
          ToolButton{
              implicitHeight: 35
              implicitWidth: 35
              iconSource: "pic/big.png"
              Text{
                  text:"放大"
                  anchors.top:parent.bottom
                  anchors.horizontalCenter: parent.horizontalCenter
              }
              onClicked: {
                  twave.zoomin()
              }

          }
          ToolButton{
              implicitHeight: 35
              implicitWidth: 35
              iconSource: "pic/small.png"
              Text{
                  text:"缩小"
                  anchors.top:parent.bottom
                  anchors.horizontalCenter: parent.horizontalCenter
              }
              onClicked: {
                  twave.zoomout()
              }
          }
          ToolButton{
              implicitHeight: 35
              implicitWidth: 35
              iconSource: "pic/reset.png"
              Text{
                  text:"重置"
                  anchors.top:parent.bottom
                  anchors.horizontalCenter: parent.horizontalCenter
              }
              onClicked: {
                  twave.zoomreset()
              }
          }

          ToolButton{
              implicitHeight: 35
              implicitWidth: 35
              iconSource: "pic/all.png"
              Text{
                  text:"全显"
                  anchors.top:parent.bottom
                  anchors.horizontalCenter: parent.horizontalCenter
              }
          }
          ToolButton{
              implicitHeight: 35
              implicitWidth: 35
              iconSource: "pic/choose.png"
              Text{
                  text:"选显"
                  anchors.top:parent.bottom
                  anchors.horizontalCenter: parent.horizontalCenter
              }
          }

          ToolButton {
              id:startbtn
              Canvas{
                  width:parent.width
                  height: parent.height
                  anchors.centerIn: parent

                  onPaint: {
                      var ctx = getContext("2d")
                      ctx.save()
                      ctx.strokeStyle = "green"
                      ctx.fillStyle = "green"
                      ctx.lineWidth = 0
                      ctx.beginPath()
                      ctx.moveTo(8,5)
                      ctx.lineTo(25,15)
                      ctx.lineTo(8,26)
                      ctx.fill()
                      ctx.stroke()
                      ctx.restore()
                  }
              }
              Text{
                  text:"开始"
                  anchors.top:parent.bottom
                  anchors.horizontalCenter: parent.horizontalCenter
              }
              onClicked: {
                  startbtn.opacity = 0.5
                  startbtn.enabled = false
                  pausebtn.opacity = 1.0
                  pausebtn.enabled = true
                  stopbtn.opacity = 1.0
                  stopbtn.enabled = true
                  //playcontrol.StartPlay()
              }
          }
          ToolButton {
              id:pausebtn
              enabled: false
              opacity: 0.5
              Canvas{
                  width:parent.width
                  height: parent.height
                  anchors.centerIn: parent

                  onPaint: {
                      var ctx = getContext("2d")
                      ctx.save()
                      ctx.strokeStyle = "green"
                      ctx.fillStyle = "green"
                      ctx.lineWidth = 3
                      ctx.beginPath()
                      ctx.moveTo(8,5)
                      ctx.lineTo(8,26)
                      ctx.moveTo(22,5)
                      ctx.lineTo(22,26)

                      ctx.stroke()
                      ctx.restore()
                  }
              }
              Text{
                  text:"暂停"
                  anchors.top:parent.bottom
                  anchors.horizontalCenter: parent.horizontalCenter
              }
              onClicked: {
                  startbtn.opacity = 1.0
                  startbtn.enabled = true
                  pausebtn.opacity = 0.5
                  pausebtn.enabled = false
                  stopbtn.opacity = 1.0
                  stopbtn.enabled = true
                  //playcontrol.PausePlay()
              }
          }
          ToolButton {
              id:stopbtn
              enabled: false
              opacity: 0.5
              Canvas{
                  width:parent.width
                  height: parent.height
                  anchors.centerIn: parent

                  onPaint: {
                      var ctx = getContext("2d")
                      ctx.save()
                      ctx.strokeStyle = "green"
                      ctx.fillStyle = "green"
                      ctx.lineWidth = 0
                      ctx.beginPath()
                      ctx.moveTo(5,5)
                      ctx.lineTo(25,5)
                      ctx.lineTo(25,25)
                      ctx.lineTo(5,25)
                      ctx.fill()
                      ctx.stroke()
                      ctx.restore()
                  }
              }
              Text{
                  text:"停止"
                  anchors.top:parent.bottom
                  anchors.horizontalCenter: parent.horizontalCenter
              }
              onClicked: {
                  startbtn.opacity = 1.0
                  startbtn.enabled = true
                  pausebtn.opacity = 0.5
                  pausebtn.enabled = false
                  stopbtn.opacity = 0.5
                  stopbtn.enabled = true
                  //playcontrol.StopPlay()
              }
          }

          Item { Layout.fillWidth: true }
      }
    }

    Rectangle{
        height: parent.height*0.8;
        width: parent.width*0.9;
        anchors.top: parent.top;
        anchors.left:parent.left;
        anchors.leftMargin: 10;
        border.width:1;
        border.color: "red";
        Text{
            id:id_text;
            anchors.top: parent.top;
            anchors.left:parent.left;
            anchors.leftMargin: 10;
            text:"时域图";
            font.pixelSize: 20;

            anchors.horizontalCenter: parent.horizontalCenter
        }

        Rectangle{

            height:parent.height*0.8;
            width: parent.width*0.9;
            anchors.top: id_text.bottom;
            anchors.topMargin:10;
            anchors.left:parent.left;
            anchors.leftMargin: 10;
            border.width:1;
            border.color: "yellow";

        }
    }

//    MouseArea {
//        anchors.fill: parent
//        onClicked: {
//            Qt.quit();
//        }
//    }

//    Text {
//        text: qsTr("Hello World")
//        anchors.centerIn: parent
//    }
}
