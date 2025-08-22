import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import craie

SplitView {
    ScrollView {
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff

        SplitView.fillWidth: true

        TextArea {
            id: text
            wrapMode: TextArea.Wrap

            background: Rectangle {
                color: parent.palette.base
            }

            BusyIndicator {
                running: Context.generating

                anchors.right: parent.right
            }
        }
    }

    Pane {
        SplitView.minimumWidth: implicitWidth
        SplitView.preferredWidth: Math.max(parent.width / 4)

        ColumnLayout {
            spacing: 10

            anchors.fill: parent

            ColumnLayout {
                Label {
                    text: "System prompt"
                }
                TextArea {
                    id: systemPrompt
                    text: Context.systemPrompt()

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }

            ColumnLayout {
                Label {
                    text: "Instructions"
                }
                TextArea {
                    id: instructions
                    text: Context.instructions()

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }

            GridLayout {
                columns: 2
                columnSpacing: 10

                Label {
                    text: "Context size"
                }
                SpinBox {
                    id: contextSize
                    to: 2 ** 31 - 1
                    value: Context.contextSize()

                    Layout.fillWidth: true
                }

                Label {
                    text: "Temperature"
                }
                SpinBoxReal {
                    id: temperature
                    value: Context.temperature()

                    Layout.fillWidth: true
                }
            }
        }
    }

    Shortcut {
        sequence: "Ctrl+Return"
        onActivated: {
            if (Context.generating) {
                Context.stopGenerating();
            } else {
                Context.setContextSize(contextSize.value);
                Context.setTemperature(temperature.value);
                Context.setInstructions(instructions.text);
                Context.setSystemPrompt(systemPrompt.text);
                Context.startGenerating(text.text);
            }
        }
    }

    Connections {
        target: Context

        function onGeneratedText(a) {
            text.insert(text.length, a);
        }
    }
}
