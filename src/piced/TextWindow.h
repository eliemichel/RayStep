#ifndef H_TEXTWINDOW
#define H_TEXTWINDOW

#include <QWidget>

class SceneTree;
class SceneTreeModel;
class NodePropertiesModel;
class UniformsModel;

namespace Ui {
	class TextWindow;
}

/**
 * TODO: It should not be this class that owns all the models
 */

class TextWindow :
	public QWidget
{
	Q_OBJECT

public:
	explicit TextWindow(QWidget *parent = 0);
	~TextWindow();

	// Arguments needed to generate the shader (to be gathered in a single accessor?)
	QString content() const;
	QString uniforms() const;

public slots:
	void load(const QString & filename);

signals:
	void submitted();

private:
	Ui::TextWindow *ui;
	SceneTree *m_scene;
	SceneTreeModel *m_model;
	NodePropertiesModel *m_propertiesModel;
	UniformsModel *m_uniformsModel;
};

#endif // H_TEXTWINDOW
