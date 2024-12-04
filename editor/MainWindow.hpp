#pragma once
#include <QMainWindow>

class QTableView;
class QListView;
class QItemSelection;
class QStandardItemModel;
class EntityModel;
class EntityPropertyModel;
class EntityManager;

// This window hold the list of entities and the details menu.
// This class manages all other views and models and the communication between them
class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr, EntityManager* entityManager = nullptr);

	~MainWindow();

private slots:
	void selectionChangedSlot(const QItemSelection& newSelection,
							  const QItemSelection& oldSelection);
private:
	EntityManager* entityManager;
	EntityModel* entityModel;
	EntityPropertyModel* entityPropertyModel;
	QListView* listView;
	QTableView* propertiesView;
};
