import QtCore
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
            id: textInput
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
                    id: systemPromptInput
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
                    id: instructionsInput
                    text: Context.instructions()

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }

            GridLayout {
                columns: 2
                columnSpacing: 10

                ComboBox {
                    id: modelInput
                    model: Context.listModels()

                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                }

                Label {
                    text: "Assistant prefix"
                }
                TextField {
                    id: assistantPrefixInput
                    text: Context.assistantPrefix()

                    Layout.fillWidth: true
                }

                Label {
                    text: "Context size"
                }
                SpinBox {
                    id: contextSizeInput
                    to: 2 ** 31 - 1
                    value: Context.contextSize()

                    Layout.fillWidth: true
                }

                Label {
                    text: "Temperature"
                }
                SpinBoxReal {
                    id: temperatureInput
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
                Context.setModel(modelInput.currentText);
                Context.setAssistantPrefix(assistantPrefixInput.text);
                Context.setContextSize(contextSizeInput.value);
                Context.setTemperature(temperatureInput.value);
                Context.setInstructions(instructionsInput.text);
                Context.setSystemPrompt(systemPromptInput.text);
                Context.startGenerating(textInput.text);
            }
        }
    }

    Connections {
        target: Context

        function onGeneratedText(text) {
            textInput.insert(textInput.length, text);
        }
    }

    Settings {
        id: settings

        property string model
        property alias assistantPrefix: assistantPrefixInput.text
        property alias contextSize: contextSizeInput.value
        property alias temperature: temperatureInput.value

        Component.onCompleted: modelInput.currentIndex = modelInput.find(model)
        Component.onDestruction: model = modelInput.currentText
    }
}
