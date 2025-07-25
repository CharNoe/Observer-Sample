#include "BookmarkUrlFactoryDialog.hpp"
#include "ui_BookmarkUrlFactoryDialog.h"

#include "core/BookmarkNode_Url.hpp"
#include <QPushButton>

namespace gui
{

BookmarkUrlFactoryDialog::BookmarkUrlFactoryDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::BookmarkUrlFactoryDialog)
{
    ui->setupUi(this);
    connect(
        ui->nameLineEdit,
        &QLineEdit::textChanged,
        this,
        &BookmarkUrlFactoryDialog::OnTextChanged
    );
    connect(
        ui->urlLineEdit,
        &QLineEdit::textChanged,
        this,
        &BookmarkUrlFactoryDialog::OnTextChanged
    );
    OnTextChanged();
    ui->nameLineEdit->setFocus();
}

BookmarkUrlFactoryDialog::~BookmarkUrlFactoryDialog()
{
    delete ui;
}

auto BookmarkUrlFactoryDialog::Execute(QWidget* parent)
    -> std::shared_ptr<core::BookmarkNode_Url>
{
    BookmarkUrlFactoryDialog dialog{parent};
    if (dialog.exec())
    {
        return std::make_shared<core::BookmarkNode_Url>(
            dialog.ui->nameLineEdit->text(), dialog.ui->urlLineEdit->text()
        );
    }
    return nullptr;
}

void BookmarkUrlFactoryDialog::OnTextChanged()
{
    auto okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (okButton)
    {
        okButton->setEnabled(
            !ui->nameLineEdit->text().isEmpty() && !ui->urlLineEdit->text().isEmpty()
        );
    }
}

} // namespace gui
