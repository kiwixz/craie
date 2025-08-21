import QtQml
import QtQuick
import QtQuick.Controls

Control {
    id: self

    property real from
    property real value
    property real to: 1

    property int decimals: 2
    property real stepSize: 1 / 10 ** decimals

    contentItem: SpinBox {
        readonly property int k: 10 ** self.decimals

        from: Math.round(self.from * k)
        value: Math.round(self.value * k)
        to: Math.round(self.to * k)

        editable: true
        stepSize: Math.round(self.stepSize * k)

        validator: DoubleValidator {
            bottom: self.from
            top: self.to
            decimals: self.decimals
        }

        textFromValue: (value, locale) => {
            return (value / k).toLocaleString(locale, "f", self.decimals);
        }

        valueFromText: (text, locale) => {
            return Math.round(Number.fromLocaleString(locale, text) * k);
        }
    }
}
