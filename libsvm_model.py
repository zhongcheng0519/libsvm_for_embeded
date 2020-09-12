import os
import re


class Model:
    def __init__(self):
        # only c_svc is supported
        self.svm_type = 'c_svc'
        # linear or polynomial is supported
        self.kernel_type = 'linear'
        # degree
        self.degree = 3
        # gamma
        self.gamma = 1
        # coef0
        self.coef0 = 0
        # class number
        self.nr_class = 2
        # total support vector number
        self.total_sv = 2
        # -b in decision function
        self.rho = 1
        # label
        self.label = [1, 0]
        # probA
        self.probA = -1
        # probB
        self.probB = 1
        # support vector number for each class
        self.nr_sv = [38, 186]

        # support vector coefs
        self.coefs = []
        # support vectors
        self.SVs = []

        # var replacement
        self.var_map = {"@{label}": "",
                        "@{coefs}": "",
                        "@{SVs}": "",
                        "@{svm_type}": "linear",
                        "@{degree}": 3,
                        "@{gamma}": 1,
                        "@{coef0}": 0,
                        "@{nr_class}": 2,
                        "@{total_sv}": 0,
                        "@{rho}": 0,
                        "@{probA}": 0,
                        "@{probB}": 0,
                        "@{vec_dim}": 0}

    def insert_newline(self, array, row_num):
        new_str = ''
        for i, value in enumerate(array):
            new_str += (value+', ')
            if i % row_num == row_num - 1:
                new_str += '\n'
        return new_str

    def refresh_var_map(self):
        self.var_map["@{label}"] = str(self.label).lstrip('[').rstrip(']')
        coefs_temp = str(self.coefs).lstrip('[').rstrip(']')
        coefs_array = coefs_temp.split(', ')
        self.var_map["@{coefs}"] = self.insert_newline(coefs_array, 10)

        for SV in self.SVs:
            self.var_map["@{SVs}"] += str(SV).lstrip('[').rstrip(']') + ',\n'
        self.var_map["@{svm_type}"] = 'linear' if self.svm_type == 'linear' else 'polynomial'
        self.var_map["@{degree}"] = str(self.degree)
        self.var_map["@{gamma}"] = str(self.gamma)
        self.var_map["@{coef0}"] = str(self.coef0)
        self.var_map["@{nr_class}"] = str(self.nr_class)
        self.var_map["@{total_sv}"] = str(self.total_sv)
        self.var_map["@{rho}"] = str(self.rho)
        self.var_map["@{probA}"] = str(self.probA)
        self.var_map["@{probB}"] = str(self.probB)
        self.var_map["@{vec_dim}"] = str(len(self.SVs[0]))

    def load(self, model_path):
        with open(model_path, 'rt') as fin:
            all_lines = fin.readlines()
        all_lines = [line.rstrip() for line in all_lines]
        sep_id = all_lines.index('SV')
        param_lines = all_lines[0:sep_id]
        sv_lines = all_lines[sep_id + 1:]

        for pl in param_lines:
            key_value = pl.split(' ')
            key = key_value[0]
            value = key_value[1:]
            if key == 'svm_type':
                self.svm_type = value[0]
            elif key == 'kernel_type':
                self.kernel_type = value[0]
            elif key == 'degree':
                self.degree = int(value[0])
            elif key == 'gamma':
                self.gamma = float(value[0])
            elif key == 'coef0':
                self.coef0 = float(value[0])
            elif key == 'nr_class':
                self.nr_class = int(value[0])
            elif key == 'total_sv':
                self.total_sv = int(value[0])
            elif key == 'rho':
                self.rho = float(value[0])
            elif key == 'label':
                self.label = [int(x) for x in value]
            elif key == 'probA':
                self.probA = float(value[0])
            elif key == 'probB':
                self.probB = float(value[0])
            elif key == 'nr_sv':
                self.nr_sv = [int(x) for x in value]
            else:
                print('Unrecognized param')

        for svl in sv_lines:
            svs =svl.split(' ')
            self.coefs.append(float(svs[0]))
            sv = [float(x.split(':')[-1]) for x in svs[1:]]
            self.SVs.append(sv)

        self.refresh_var_map()

    def fprint_c(self, c_file_in):
        name_ext = os.path.splitext(c_file_in)
        if not os.path.exists(c_file_in):
            print('%s does not exist.' % c_file_in)
            return False
        if name_ext[-1] == '.in' and os.path.splitext(name_ext[0])[-1] == '.c':
            src_name = c_file_in
            dst_name = name_ext[0]
            # find @{varname} in src file, replace them, and generate new file
            with open(src_name, 'rt') as f:
                lines = f.readlines()
                new_lines = list()
                for line in lines:
                    pattern = re.compile(r'@{\w+}')
                    rep_vars = pattern.findall(line)
                    for rep_var in rep_vars:
                        if rep_var in self.var_map:
                            line = line.replace(rep_var, self.var_map[rep_var])
                        else:
                            print('%s not in var_map' % rep_var)
                    new_lines.append(line)
            with open(dst_name, 'wt') as f:
                f.writelines(new_lines)
                print('Written to %s' % dst_name)

            return True
        else:
            print('filename.c.in is expected')
            return False
