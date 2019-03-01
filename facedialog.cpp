#include "facedialog.h"

const QStringList FaceDialog::emojiList = QStringList()
        << "😚" << "😓" << "😰" << "😝" << "😁" << "😜" << "☺"
        << "😉" << "😍" << "😔" << "😄" << "😏" << "😒" << "😳"
        << "😘" << "😭" << "😱" << "😂" << "💪" << "👊" << "👍"
        << "☝" << "👏" << "✌" << "👎" << "🙏" << "👌" << "👈"
        << "👉" << "👆" << "👇" << "👀" << "👃" << "👄" << "👂"
        << "🍚" << "🍝" << "🍜" << "🍙" << "🍧" << "🍣" << "🎂"
        << "🍞" << "🍔" << "🍳" << "🍟" << "🍺" << "🍻" << "🍸"
        << "☕" << "🍎" << "🍊" << "🍓" << "🍉" << "💊" << "🚬"
        << "🎄" << "🌹" << "🎉" << "🌴" << "💝" << "🎀" << "🎈"
        << "🐚" << "💍" << "💣" << "👑" << "🔔" << "⭐" << "✨"
        << "💨" << "💦" << "🔥" << "🏆" << "💰" << "💤" << "⚡"
        << "👣" << "💩" << "💉" << "♨" << "📫" << "🔑" << "🔒"
        << "✈" << "🚄" << "🚗" << "🚤" << "🚲" << "🐎" << "🚀"
        << "🚌" << "⛵" << "👩" << "👨" << "👧" << "👦" << "🐵"
        << "🐙" << "🐷" << "💀" << "🐤" << "🐨" << "🐮" << "🐔"
        << "🐸" << "👻" << "🐛" << "🐠" << "🐶" << "🐯" << "👼"
        << "🐧" << "🐳" << "🐭" << "👒" << "👗" << "💄" << "👠"
        << "👢" << "🌂" << "👜" << "👙" << "👕" << "👟" << "☁"
        << "☀" << "☔" << "🌙" << "⛄" << "⭕" << "❌" << "❔"
        << "❕" << "☎" << "📷" << "📱" << "📠" << "💻" << "🎥"
        << "🎤" << "🔫" << "💿" << "💓" << "♣" << "🀄" << "〽"
        << "🎰" << "🚥" << "🚧" << "🎸" << "💈" << "🛀" << "🚽"
        << "🏠" << "⛪" << "🏦" << "🏥" << "🏨" << "🏧" << "🏪"
        << "🚹" << "🚺";

FaceDialog::FaceDialog(QWidget *parent)
{
    this->setParent(parent, Qt::Dialog);

    // 7*23+2 = 163 = 13*13-6
    QHBoxLayout *faceDialogLayout = new QHBoxLayout(this);
    QTableWidget *tableWidget = new QTableWidget(13, 13, this);
    tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWidget->horizontalHeader()->hide();
    tableWidget->verticalHeader()->hide();
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    faceDialogLayout->addWidget(tableWidget);

    for(int i = 0;i < emojiList.length();i++)
    {
        QTableWidgetItem *temp = new QTableWidgetItem(emojiList.at(i));
        temp->setTextAlignment(Qt::AlignCenter);
        temp->setFlags(temp->flags()^Qt::ItemIsEditable);
        tableWidget->setItem(i/13, i%13, temp);
    }
    int width = tableWidget->rowHeight(0);
    for(int i = 0;i < 13;i++)
    {
        tableWidget->setColumnWidth(i, width);
    }

    int dialogWidth = tableWidget->horizontalHeader()->length()+4;
    int dialogHeight = tableWidget->verticalHeader()->length()+4;
    tableWidget->setFixedSize(dialogWidth, dialogHeight);

    faceDialogLayout->setMargin(0);
    faceDialogLayout->setSpacing(0);
    this->setLayout(faceDialogLayout);
    this->setWindowTitle("emoji 表情");
    this->setWindowIcon(QIcon::fromTheme("smiley-shape"));
    this->setWindowModality(Qt::ApplicationModal);
    this->setAttribute(Qt::WA_DeleteOnClose);
    connect(tableWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
            this, SLOT(dialogFinish(QTableWidgetItem *)));
}

void FaceDialog::dialogFinish(QTableWidgetItem *item)
{
    emit dialogFinished(item->text());
    this->accept();
}
