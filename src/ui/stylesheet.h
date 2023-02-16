//
// Created by egonr on 2/16/2023.
//

#ifndef WINCAP11_STYLESHEET_H
#define WINCAP11_STYLESHEET_H
auto stylesheet = QString(R"(
* {
    background: #111;
    color: #ddd;
}

QWidget::item:selected {
    background: #4040ff;
}

QPushButton {
    border: 1px solid #bbb;
    padding: 4px 0px;
    border-radius: 4px;
    background-color: #191919;
    min-width: 80px;
}
QPushButton:hover {
    border: 1px solid #eee;
    background-color: #222;
}
QPushButton:focus {
    border: 1px solid #4040ff;
    outline: none;
}
QPushButton:disabled {
    background-color: #666;
    color: #222;
}
QPushButton:pressed {
    background-color: #333333;
}

QComboBox {
    border: 1px solid #bbb;
    padding: 3px 8px;
    background-color: #191919;
    min-width: 80px;
}

QSpinBox {
    border: 1px solid #bbb;
    padding: 3px 8px;
    background-color: #191919;
    min-width: 80px;
})");
#endif //WINCAP11_STYLESHEET_H
