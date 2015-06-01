#include "nimcompilerbuildstepconfigwidget.h"
#include "nimcompilerbuildstep.h"
#include "ui_nimcompilerbuildstepconfigwidget.h"
#include "nimproject.h"

namespace NimPlugin {

NimCompilerBuildStepConfigWidget::NimCompilerBuildStepConfigWidget(NimCompilerBuildStep *buildStep)
    : ProjectExplorer::BuildStepConfigWidget()
    , m_buildStep(buildStep)
    , m_ui(new Ui::NimCompilerBuildStepConfigWidget())
{
    m_ui->setupUi(this);
    connectBuildStepSignals();
    connectUISignals();
    updateUI();
}

QString NimCompilerBuildStepConfigWidget::summaryText() const
{
    return QLatin1String("Nim Compiler build step");
}

QString NimCompilerBuildStepConfigWidget::displayName() const
{
    return QLatin1String("Nim Compiler");
}

void NimCompilerBuildStepConfigWidget::connectUISignals()
{
    connect(m_ui->targetComboBox, SIGNAL(activated(int)), this, SLOT(onTargetChanged(int)));
    connect(m_ui->additionalArgumentsLineEdit, SIGNAL(textEdited(QString)),
            this, SLOT(onAdditionalArgumentsTextEdited(QString)));
}

void NimCompilerBuildStepConfigWidget::updateBuildDirectory()
{
    auto workingDir = m_buildStep->processParameters()->workingDirectory();
    m_ui->buildDirectoryLineEdit->setText(workingDir);
}

void NimCompilerBuildStepConfigWidget::onTargetChanged(int index)
{
    Q_UNUSED(index);
    auto data = m_ui->targetComboBox->currentData();
    Utils::FileName path = Utils::FileName::fromString(data.toString());
    m_buildStep->setTarget(path);
}

void NimCompilerBuildStepConfigWidget::connectBuildStepSignals()
{
    connect(m_buildStep, SIGNAL(targetChanged(Utils::FileName)), this, SLOT(updateUI()));
    connect(m_buildStep, SIGNAL(additionalArgumentsChanged(QString)), this, SLOT(updateUI()));
}

void NimCompilerBuildStepConfigWidget::updateUI()
{
    updateBuildDirectory();
    updateCommandLineText();
    updateTargetComboBox();
}

void NimCompilerBuildStepConfigWidget::onAdditionalArgumentsTextEdited(const QString &text)
{
    m_buildStep->setAdditionalArguments(text);
}

void NimCompilerBuildStepConfigWidget::updateCommandLineText()
{
    auto parameters = m_buildStep->processParameters();

    QStringList command;
    command << parameters->command();
    command << parameters->arguments();

    // Remove empty args
    auto predicate = [](const QString& str)->bool{return str.isEmpty();};
    auto it = std::remove_if(command.begin(), command.end(), predicate);
    command.erase(it, command.end());

    m_ui->commandTextEdit->setText(command.join(QChar::LineFeed));
}

void NimCompilerBuildStepConfigWidget::updateTargetComboBox()
{
    using namespace ProjectExplorer;

    auto project = dynamic_cast<NimProject*>(m_buildStep->project());

    // Save current selected file
    QVariant currentFile;
    if (m_ui->targetComboBox->currentIndex() != -1)
        currentFile = m_ui->targetComboBox->currentData();

    // Re enter the files
    m_ui->targetComboBox->clear();
    for (Utils::FileName filename : project->nimFiles())
        m_ui->targetComboBox->addItem(filename.fileName(), filename.toString());

    // Reselect the previous file
    if (currentFile.isValid()) {
        int index = m_ui->targetComboBox->findData(currentFile);
        if (index > 0)
            m_ui->targetComboBox->setCurrentIndex(index);
    }
}

}

