import QtQuick
import QtQuick.Controls 2.5
import Qt.labs.qmlmodels 1.0
import QtQml
import QtQuick.Window 2.3
import QtQuick.Controls 2.2 as Quick2


Item {
    property int curIndex: 0
    property int curIndexWitouthZero: 0
    SwipeView {
        id: view
        objectName: "swipeview"
        currentIndex: 1
        width: parent.width
        height: parent.height
        spacing: 5
        orientation: Qt.Horizontal
                //#171c1f
        background: Rectangle {
            color: "#171c1f"
        }
        contentItem: ListView {
                  model: view.contentModel
                  interactive: view.interactive
                  currentIndex: view.currentIndex
                  //color: "#171c1f"

                  spacing: view.spacing
                  orientation: view.orientation
                  snapMode: ListView.SnapOneItem
                  boundsBehavior: Flickable.StopAtBounds

                  highlightRangeMode: ListView.StrictlyEnforceRange
                  preferredHighlightBegin: 0
                  preferredHighlightEnd: 0
                  highlightMoveDuration: 250
                  //                    min:10

                  maximumFlickVelocity: 4 * (view.orientation ===
                  Qt.Horizontal ? width : height)
              }

        Component.onCompleted: {
                   curIndexWitouthZero = view.currentIndex
                   curIndexWitouthZero += 1
                   addPage(createPage())
               }

        onCurrentIndexChanged: {
            curIndexWitouthZero = view.currentIndex
            curIndexWitouthZero += 1
        }

        function createPage(){
            var component = Qt.createComponent("schedule.qml");
            var page = component.createObject(view);
            return page
        }

        function addPage(page) {
            addItem(page)
            page.visible = true
        }

        function addPageSignal() {
            view.addPage(createPage())
            if(schedule.variantList[0]===Qt.formatDateTime(new Date(), "dd.MM.yyyy"))
                curIndex = view.count
        }

        function setcurindex()
        {
            currentIndex = curIndex-2
        }

        function remove_funtom()
        {
            takeItem(0)
        }

        function next_page()
        {
            if(currentIndex!=count-1)
                currentIndex+=1
        }

        function previous_page()
        {
            if(currentIndex!=0)
                currentIndex-=1
        }

        function delete_all()
        {
            //currentIndex = 0
            for(var i=0;i<count;i++)
            {
                //takeItem(i)
                console.log("del",i);
                view.removeItem(view.itemAt(i))
            }
            view.update()
        }
    }
}
