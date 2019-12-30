## 概要

このプログラム、サークルfuzziliaがコミックマーケットにサークル参観する際に作成したデジタルお絵かき用キー入力デバイス、通称左手デバイスのファームウェアです。
対象のSoCはESP32で、プログラムのビルド、書き込みはArduinoで行います。

これを設定するためのPC側のアプリケーション https://github.com/fuzzilia/sh-config とセットで機能します。

## VSCodeで開発する

- 予めVSCodeとArduinoIDEが入っていること。
    - ArduinoIDEはストア版だとVSCodeが自動認識しない模様。インストーラーで入れるの推奨。
- VSCodeにArudinoプラグインをインストール
    - [Microsoftが出してるやつ](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino)
- VSCodeの設定(settings.json)に下記を追記
    - ```json
      "arduino.additionalUrls": "https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json"
      ```
    - arduino.additionalUrls : esp32に対応させる
- コマンドパレットから Arduino Board Managerを呼び出し、ESP32を選択、Install
- 画面右下から Select Board でESP32 Dev Moduleを選択
- コマンドパレットから Arduino Upload でプログラム書き込み

## LICENSE

MIT