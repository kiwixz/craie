import QtQuick.Controls
import QtQuick.Layouts

import craie

Pane {
    RowLayout {
        anchors.fill: parent
        spacing: 10

        TextArea {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        ColumnLayout {
            Layout.maximumWidth: Math.max(parent.width / 5, implicitWidth)

            Label {
                text: "System prompt"
            }
            TextArea {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.bottomMargin: 10
            }

            Label {
                text: "Instructions"
            }
            TextArea {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.bottomMargin: 10
            }

            GridLayout {
                columns: 2
                columnSpacing: 10

                Label {
                    text: "Temperature"
                }
                SpinBoxReal {
                    Layout.fillWidth: true
                }
            }
        }
    }
}
