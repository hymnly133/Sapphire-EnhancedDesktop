#ifndef ELANAVIGATIONBAR_H
#define ELANAVIGATIONBAR_H

#include <QWidget>

#include "Def.h"
#include "stdafx.h"

class ElaNavigationBarPrivate;
class ELA_EXPORT ElaNavigationBar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaNavigationBar)
public:
    explicit ElaNavigationBar(QWidget* parent = nullptr);
    ~ElaNavigationBar();
    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(QPixmap pix);
    void setUserInfoCardTitle(QString title);
    void setUserInfoCardSubTitle(QString subTitle);

    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, ElaIconType awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, ElaIconType = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, ElaIconType awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, ElaIconType awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, int keyPoints = 0, ElaIconType awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints = 0, ElaIconType awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QString& footerKey, int keyPoints = 0, ElaIconType awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, ElaIconType awesome = ElaIconType::None) const;

    void setNodeKeyPoints(QString nodeKey, int keyPoints);
    int getNodeKeyPoints(QString nodeKey) const;

    void navigation(QString pageKey, bool isLogClicked = true);
    void setDisplayMode(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation = true);

Q_SIGNALS:
    Q_SIGNAL void userInfoCardClicked();
    Q_SIGNAL void navigationNodeClicked(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey);
    Q_SIGNAL void navigationNodeAdded(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
};

#endif // ELANAVIGATIONBAR_H
