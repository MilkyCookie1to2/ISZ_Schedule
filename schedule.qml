import QtQuick
import QtQuick.Controls
import Qt.labs.qmlmodels
import QtQuick.Layouts
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Window 2.3

    TableView
    {
        property int curW: 300
        objectName: "tableview"
        implicitWidth: curW
        columnSpacing : 5
        rowSpacing : 5
        clip: true

        model: TableModel {
            id: model_schedule
            TableModelColumn { display: "time" }
            TableModelColumn { display: "subject" }
        }

        Component.onCompleted:{
            curW = schedule.for_widtho
            console.log(schedule.for_widtho)
            for (var i = 0; i < schedule.variantList.length; i+=2)
            {
                model_schedule.appendRow({'time': schedule.variantList[i], 'subject': schedule.variantList[i+1]})
            }
        }

        columnWidthProvider: function (column) {
            return column===0 ? 100 : curW-105
        }

        delegate: Rectangle {
            implicitHeight: stringTxt.paintedHeight
            implicitWidth: columnWidthProvider(column)
            radius: 10
            color: row===0 ? "#bde7ff" : "#2c3134"
            Rectangle
            {
                implicitHeight: parent.height
                implicitWidth: columnWidthProvider(column)-10
                color: row===0 ? "#bde7ff" : "#2c3134"
                anchors.left: column === 0 ? undefined : parent.left;
                anchors.right: column === 0  ? parent.right : undefined;
                Text{
                    id: stringTxt
                    text: display
                    color: row === 0 ? "#041a25" : "#edf2f6"
                    width: column!==1 ? undefined : row!==0 ? columnWidthProvider(column)-30 : undefined
                    //anchors.leftMargin: 10
                    anchors.centerIn: parent
                    wrapMode: Text.WordWrap
                }
                Rectangle{
                    width: 10
                    x: parent.width-5
                    y: 5
                    radius: 5
                    implicitHeight: parent.height-10
                    color: column===0 ? "transparent": stringTxt.text.search("ПЗ") !== -1 ? "yellow" : stringTxt.text.search("Лек.") !==-1 ? "green" : "transparent"
                }
            }
        }
    }
/*Rectangle
{
    width: 100
    height: 100
    color: Qt.rgba(Math.random(), Math.random(), Math.random(), Math.random())
}*/
