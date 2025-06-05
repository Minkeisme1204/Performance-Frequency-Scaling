
plot_tree(clf, feature_names=X.columns, class_names=[str(cls) for cls in clf.classes_], filled=True)
plt.title("Decision Tree for CPU Frequency Prediction")
plt.show()