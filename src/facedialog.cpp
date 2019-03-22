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
    QGridLayout *faceDialogLayout = new QGridLayout(this);
    //faceDialogLayout->setMargin(0);
    //faceDialogLayout->setSpacing(0);
    this->setWindowTitle("emoji 表情");
    this->setWindowIcon(QIcon::fromTheme("smiley-shape"));
    this->setWindowModality(Qt::ApplicationModal);
    this->setAttribute(Qt::WA_DeleteOnClose);

    QLabel temp;
    QFont font = temp.font();
    font.setPointSize(20);
    for(int i = 0;i < emojiList.length();i++)
    {
        QLabel *temp = new QLabel(emojiList.at(i), this);
        temp->setToolTip(emojiList.at(i));
        temp->setFont(font);
        temp->setAlignment(Qt::AlignCenter);
        faceDialogLayout->addWidget(temp, i/13, i%13);
    }
}

void FaceDialog::mousePressEvent(QMouseEvent *event)
{
    if(this->childAt(event->pos()) != nullptr)
    {
        emit dialogFinished(static_cast<QLabel *>(
                                this->childAt(event->pos()))->text());
        this->accept();
    }
}
