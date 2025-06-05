import pandas as pd
from sklearn.tree import DecisionTreeClassifier, export_text, plot_tree
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, accuracy_score, confusion_matrix
import matplotlib.pyplot as plt
import seaborn as sns

df = pd.read_csv("/home/minkeislinux/Desktop/Workplace/HUST/04_He_dieu_hanh/Performance-Frequency-Scaling/src/logger/logged_data/data.csv")

# Chia feature và target
X = df.drop("frequency", axis=1)
y = df["frequency"]

# Tách train/test
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Huấn luyện Decision Tree
clf = DecisionTreeClassifier(max_depth=5, random_state=42)
clf.fit(X_train, y_train)

# Dự đoán và đánh giá
y_pred = clf.predict(X_test)
print("Accuracy:", accuracy_score(y_test, y_pred))
print("Classification Report:\n", classification_report(y_test, y_pred))

# In cấu trúc cây dạng text
print("\nCây quyết định:")
print(export_text(clf, feature_names=list(X.columns)))

# Vẽ sơ đồ cây
plt.figure(figsize=(20, 10))
plot_tree(clf, feature_names=X.columns, class_names=[str(cls) for cls in clf.classes_], filled=True)
plt.title("Decision Tree for CPU Frequency Prediction")
plt.show()

# ---- VẼ BẰNG MATPLOTLIB ----

report = classification_report(y_test, y_pred, output_dict=True)
conf_matrix = confusion_matrix(y_test, y_pred, labels=clf.classes_)
# 1. Vẽ confusion matrix
plt.figure(figsize=(10, 6))
sns.heatmap(conf_matrix, annot=True, fmt='d', cmap='Blues',
            xticklabels=clf.classes_, yticklabels=clf.classes_)
plt.title('Confusion Matrix')
plt.xlabel('Predicted')
plt.ylabel('True')
plt.show()

# 2. Vẽ classification report
report_df = pd.DataFrame(report).transpose()
fig, ax = plt.subplots(figsize=(20, 6))
ax.axis('off')
tbl = ax.table(cellText=report_df.round(2).values,
               colLabels=report_df.columns,
               rowLabels=report_df.index,
               loc='center',
               cellLoc='center')
tbl.auto_set_font_size(False)
tbl.set_fontsize(10)
tbl.scale(1.2, 1.5)
plt.title("Classification Report", fontsize=14)
plt.show()