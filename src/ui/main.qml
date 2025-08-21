import QtQuick.Controls
import QtQuick.Layouts

import craie

Pane {
    SplitView {
        TextArea {
            SplitView.fillWidth: true
        }

        ColumnLayout {
            spacing: 10

            SplitView.preferredWidth: Math.max(parent.width / 4, implicitWidth)

            ColumnLayout {
                Label {
                    text: "System prompt"
                }
                TextArea {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }

            ColumnLayout {
                Label {
                    text: "Instructions"
                }
                TextArea {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
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
