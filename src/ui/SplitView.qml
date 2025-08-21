import QtQuick
import QtQuick.Controls

SplitView {
    anchors.fill: parent
    handle: Item {
        implicitWidth: 10
    }

    implicitHeight: contentChildren.reduce((a, b) => Math.max(a, b.implicitHeight), 0)
    implicitWidth: contentChildren.reduce((a, b) => a + b.implicitWidth, (contentChildren.length - 1) * 10)

    onContentChildrenChanged: () => {
        for (let child of contentChildren) {
            child.SplitView.minimumWidth = Qt.binding(() => child.implicitWidth);
            child.SplitView.minimumHeight = Qt.binding(() => child.implicitHeight);
        }
    }
}
