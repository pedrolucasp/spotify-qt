import com.kraxarn.utils 1.0
import com.kraxarn.settings 1.0

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

import "js/setup.js" as JS

ApplicationWindow {
	id: root
	title: "spotify-qt-quick"
	visible: true
	width: 540
	height: 960
	Material.theme: Material.System

	Settings {
		id: settings
	}

	Utils {
		id: utils
	}

	OpenLinkDialog {
		id: openLinkDialog
		width: root.width * 0.9
	}

	ColumnLayout {
		anchors {
			left: parent.left
			right: parent.right
			top: parent.top
			margins: 16
		}
		Label {
			text: "Welcome to spotify-qt-quick!"
			font.pointSize: 16
		}
		Label {
			Layout.topMargin: 16
			text: JS.getWelcomeText()
		}
		Label {
			text: "Client ID"
			Layout.topMargin: 16
		}
		TextField {
			id: clientId
			Layout.fillWidth: true
			text: settings.settings.Account.client_id
		}
		Label {
			text: "Client Secret"
			Layout.topMargin: 16
		}
		TextField {
			id: clientSecret
			Layout.fillWidth: true
			text: settings.settings.Account.client_secret
		}
	}

	ColumnLayout {
		anchors {
			left: parent.left
			right: parent.right
			bottom: parent.bottom
			leftMargin: 24
			rightMargin: 24
			bottomMargin: 24
		}
		Button {
			text: "Cancel"
			Layout.fillWidth: true
			onClicked: root.close()
		}
		Button {
			text: "Spotify Dashboard"
			Layout.fillWidth: true
			onClicked: JS.openDashboard()
		}
		Button {
			text: "Authenticate"
			Layout.fillWidth: true
		}
	}
}