#ifndef HELP_H
#define HELP_H

/**
 * @class Help
 * @brief A simple dialog for displaying information about how to use editor.
 *
 * @authors Ryan Hamerman, Steven Luo, Julia Maia, Yudith Mendoza, William Ngo, Elijah Potter
 * @date 3/31/2025
 *
 * Checked by Yudith Mendoza
 */

#include <QDialog>

namespace Ui {
class Help;
}

class Help : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Help dialog
     */
    explicit Help(QWidget *parent = nullptr);

    /**
     * @brief Desconstructor for the Help Dialog
     */
    ~Help();

private:
    Ui::Help *ui;
};

#endif // HELP_H
